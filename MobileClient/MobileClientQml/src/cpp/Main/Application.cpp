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

Application::Application(int argc, char *argv[])
  : QApplication(argc, argv)
  , m_Settings              (nullptr)
  , m_PlateformDetail       (nullptr)
  , m_ApplicationHelper     (nullptr)
  , m_LocationManager       (nullptr)
  , m_PictureCacher         (nullptr)
  , m_CurrentUser           (nullptr)
  , m_PictureUploader       (nullptr)
  , m_QQmlApplicationEngine (nullptr)
{
  // Application informations
  QApplication::setOrganizationName   ("Lowerspot");
  QApplication::setOrganizationDomain ("lowerspot.com");
  QApplication::setApplicationName    ("Lowerspot");
  QApplication::setApplicationVersion ("V0.2.0");

  // Settings
  m_Settings = new Settings(this);

  // Logger
  Logger::instanziate(Logger::LOG_DEBUG);
  Logger::instance()->setLogLevel(m_Settings->get_Logger_LogLevel());

  // Plateform detail
  m_PlateformDetail = new PlateformDetail(this);

  // Application helper
  m_ApplicationHelper = new ApplicationHelper(m_Settings,
                                              m_PlateformDetail);
  m_ApplicationHelper->setDevelopmentMode(false);
  m_ApplicationHelper->startupTimerStart();

  m_LocationManager = new LocationManager(m_Settings,
                                          m_PlateformDetail);
  m_PictureCacher = new PictureCacher(this);

  // Repositories
  PictureRepository::instanziate();
  SpotRepository::instanziate(m_LocationManager);
  UserRepository::instanziate();

  m_CurrentUser = new CurrentUser(m_Settings,
                                  this);
  m_PictureUploader = new PictureUploader(this);

  // Custom network access factory
  m_UltraQmlAccessManagerFactory  = new UltraQmlAccessManagerFactory();

  // QML Engine
  m_QQmlApplicationEngine = new QQmlApplicationEngine();

  m_QQmlApplicationEngine->setNetworkAccessManagerFactory(m_UltraQmlAccessManagerFactory);


  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Application",       m_ApplicationHelper);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_PlateformDetail",   m_PlateformDetail);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_LocationManager",   m_LocationManager);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_PictureCacher",     m_PictureCacher);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Logger",            Logger::instance());
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Settings",          m_Settings);

  m_QQmlApplicationEngine->rootContext()->setContextProperty("wa_CurrentUser",       m_CurrentUser);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("wa_PictureUploader",   m_PictureUploader);

  m_QQmlApplicationEngine->rootContext()->setContextProperty("re_PictureRepository", PictureRepository::instance());
  m_QQmlApplicationEngine->rootContext()->setContextProperty("re_SpotRepository",    SpotRepository::instance());
  m_QQmlApplicationEngine->rootContext()->setContextProperty("re_UserRepository",    UserRepository::instance());

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
  m_QQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
  // Delete objects
  delete m_QQmlApplicationEngine;
  delete m_PictureUploader;
  delete m_CurrentUser;
  delete m_PictureCacher;
  delete m_LocationManager;
  delete m_ApplicationHelper;
  delete m_PlateformDetail;
  delete m_Settings;

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
    m_LocationManager->suspendUpdates();

    saveSettings();
  }
  else if(applicationState == Qt::ApplicationActive)
  {
    Logger::info("Application active.");

    // Resume gps updates
    m_LocationManager->resumeUpdates();
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::slot_QApplication_aboutToQuit()
{
  Logger::info("Application closing...");

  // Suspend gps updates
  m_LocationManager->suspendUpdates();

  saveSettings();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::saveSettings()
{
  // Set and save settings
  m_Settings->set_Application_LastVersion(QApplication::applicationVersion());
  m_Settings->set_Logger_LogLevel(Logger::instance()->getLogLevel());
  m_Settings->set_Location_LastCoordinate(m_LocationManager->coordinate());
  m_Settings->sync();
}
