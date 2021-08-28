/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      01.05.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "Application.h"

// Projects includes -----------------------
#include "../HelperClasses/ApplicationHelper.h"
#include "../HelperClasses/PlateformDetail.h"
#include "../HelperClasses/LocationManager.h"
#include "../HelperClasses/PictureCacher.h"
#include "../HelperClasses/UltraQmlAccessManagerFactory.h"
#include "../Settings/Settings.h"
#include "../WebApi/WebApi.h"
#include "../WebApi/SpotRepository.h"
#include "../WebApi/SpotsModel.h"
#include "../WebApi/Spot.h"
#include "../WebApi/PictureRepository.h"
#include "../WebApi/PicturesModel.h"
#include "../WebApi/Picture.h"
#include "../WebApi/UserRepository.h"
#include "../WebApi/User.h"
#include "../WebApi/CurrentUser.h"
#include "../WebApi/PictureUploader.h"

// Library includes -----------------------
#include <HelperClasses/Exception.h>
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Application::CONST::SERVER::DEFAULT_ADDRESS("http://lowerspot.com");

//-----------------------------------------------------------------------------------------------------------------------------

Application::Application(int argc, char *argv[])
  : QApplication(argc, argv)
  , mSettings              (nullptr)
  , mPlateformDetail       (nullptr)
  , mApplicationHelper     (nullptr)
  , mLocationManager       (nullptr)
  , mPictureCacher         (nullptr)
  , mCurrentUser           (nullptr)
  , mPictureUploader       (nullptr)
  , mQQmlApplicationEngine (nullptr)
{
  // Application informations
  QApplication::setOrganizationName   ("Lowerspot");
  QApplication::setOrganizationDomain ("lowerspot.com");
  QApplication::setApplicationName    ("Lowerspot");
  QApplication::setApplicationVersion ("V0.3.0");

  // Settings
  mSettings = new Settings(this);

  // Logger
  Logger::instanziate(Logger::LOG_DEBUG);
  Logger::instance()->setLogLevel(mSettings->get_Logger_LogLevel());

  // Server address
  WebApi::instance()->setServerAddress(CONST::SERVER::DEFAULT_ADDRESS);

  // Command line arguments
  parseCommandLineArguments();

  // Plateform detail
  mPlateformDetail = new PlateformDetail(this);

  // Application helper
  mApplicationHelper = new ApplicationHelper(mSettings,
                                             mPlateformDetail);
  mApplicationHelper->startupTimerStart();

  mLocationManager = new LocationManager(mSettings,
                                         mPlateformDetail);
  mPictureCacher = new PictureCacher(this);

  // Repositories
  PictureRepository::instanziate();
  SpotRepository::instanziate(mLocationManager);
  UserRepository::instanziate();

  mCurrentUser = new CurrentUser(mSettings,
                                  this);
  mPictureUploader = new PictureUploader(this);

  // Custom network access factory
  mUltraQmlAccessManagerFactory  = new UltraQmlAccessManagerFactory();

  // QML Engine
  mQQmlApplicationEngine = new QQmlApplicationEngine();

  mQQmlApplicationEngine->setNetworkAccessManagerFactory(mUltraQmlAccessManagerFactory);


  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_Application",       mApplicationHelper);
  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_PlateformDetail",   mPlateformDetail);
  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_LocationManager",   mLocationManager);
  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_PictureCacher",     mPictureCacher);
  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_Logger",            Logger::instance());
  mQQmlApplicationEngine->rootContext()->setContextProperty("hc_Settings",          mSettings);

  mQQmlApplicationEngine->rootContext()->setContextProperty("wa_CurrentUser",       mCurrentUser);
  mQQmlApplicationEngine->rootContext()->setContextProperty("wa_PictureUploader",   mPictureUploader);

  mQQmlApplicationEngine->rootContext()->setContextProperty("re_PictureRepository", PictureRepository::instance());
  mQQmlApplicationEngine->rootContext()->setContextProperty("re_SpotRepository",    SpotRepository::instance());
  mQQmlApplicationEngine->rootContext()->setContextProperty("re_UserRepository",    UserRepository::instance());

  qmlRegisterType<PicturesModel>   ("PicturesModel",    1, 0, "PicturesModel");
  qmlRegisterType<Picture>         ("Picture",          1, 0, "Picture");
  qmlRegisterType<SpotsModel>      ("SpotsModel",       1, 0, "SpotsModel");
  qmlRegisterType<Spot>            ("Spot",             1, 0, "Spot");
  qmlRegisterType<User>            ("User",             1, 0, "User");

//  QObject::connect(this,
//                   SIGNAL(applicationStateChanged(Qt::ApplicationState)),
//                   this,
//                   SLOT(slot_QApplication_applicationStateChanged(Qt::ApplicationState)));
//  QObject::connect(this,
//                   SIGNAL(aboutToQuit()),
//                   this,
//                   SLOT(slot_QApplication_aboutToQuit()));

  Logger::info("Load main.qml...");
  mQQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
  // Delete objects
  delete mQQmlApplicationEngine;
  delete mPictureUploader;
  delete mCurrentUser;
  delete mPictureCacher;
  delete mLocationManager;
  delete mApplicationHelper;
  delete mPlateformDetail;
  delete mSettings;

  // Destroy singletons
  Logger::destroy();
  UserRepository::destroy();
  PictureRepository::destroy();
  SpotRepository::destroy();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::slot_QApplication_applicationStateChanged(Qt::ApplicationState applicationState)
{
  if(   applicationState == Qt::ApplicationSuspended
     || applicationState == Qt::ApplicationHidden)
  {
    Logger::info("Application suspended.");

    // Suspend gps updates
    mLocationManager->suspendUpdates();

    saveSettings();
  }
  else if(applicationState == Qt::ApplicationActive)
  {
    Logger::info("Application active.");

    // Resume gps updates
    mLocationManager->resumeUpdates();
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::slot_QApplication_aboutToQuit()
{
  Logger::info("Application closing...");

  // Suspend gps updates
  mLocationManager->suspendUpdates();

  saveSettings();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::parseCommandLineArguments()
{
  QCommandLineParser qCommandLineParser;
  qCommandLineParser.setApplicationDescription(QApplication::applicationName());
  qCommandLineParser.addHelpOption();
  qCommandLineParser.addVersionOption();

  // Development mode
  QCommandLineOption qCommandLineOption_serverAddress(QStringList() << "s" << "server-address",
                                                      "Server address.",
                                                      "address",
                                                      "");
  qCommandLineParser.addOption(qCommandLineOption_serverAddress);

  QCommandLineOption qCommandLineOption_serverPort(QStringList() << "p" << "server-port",
                                                   "Server port",
                                                   "port",
                                                   "-1");
  qCommandLineParser.addOption(qCommandLineOption_serverPort);

  qCommandLineParser.process(QApplication::arguments());

  if(qCommandLineParser.value(qCommandLineOption_serverAddress).isEmpty() == false)
    WebApi::instance()->setServerAddress(qCommandLineParser.value(qCommandLineOption_serverAddress),
                                         qCommandLineParser.value(qCommandLineOption_serverPort).toInt());
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::saveSettings()
{
  // Set and save settings
  mSettings->set_Application_LastVersion(QApplication::applicationVersion());
  mSettings->set_Logger_LogLevel(Logger::instance()->getLogLevel());
  mSettings->set_Location_LastCoordinate(mLocationManager->coordinate());
  mSettings->sync();
}
