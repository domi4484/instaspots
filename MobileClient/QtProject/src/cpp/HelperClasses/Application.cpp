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
#include "ApplicationHelper.h"
#include "Logger.h"
#include "PlateformDetail.h"
#include "LocationManager.h"
#include "PictureCacher.h"
#include "../Settings/Settings.h"
#include "../WebApi/SpotRepository.h"
#include "../WebApi/SpotsModel.h"
#include "../WebApi/Spot.h"
#include "../WebApi/PictureRepository.h"
#include "../WebApi/PicturesModel.h"
#include "../WebApi/Picture.h"
#include "../WebApi/User.h"
#include "../WebApi/PictureUploader.h"

// Qt includes -----------------------------
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Application::CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE("developmentMode");

//-----------------------------------------------------------------------------------------------------------------------------

Application::Application(int argc, char *argv[]) :
  QApplication(argc, argv),
  m_Settings              (NULL),
  m_PlateformDetail       (NULL),
  m_ApplicationHelper     (NULL),
  m_LocationManager       (NULL),
  m_PictureCacher         (NULL),
  m_User                  (NULL),
  m_PictureUploader       (NULL),
  m_QQmlApplicationEngine (NULL)
{
  // Application informations
  QApplication::setOrganizationName   ("Lowerspot");
  QApplication::setOrganizationDomain ("lowerspot.com");
  QApplication::setApplicationName    ("Lowerspot");
  QApplication::setApplicationVersion ("V0.0.6");

  // Command line arguments
  QMap<QString, QVariant> qMap_Arguments = parseCommandLineArguments();


  // Settings
  m_Settings = new Settings(this);

  // Logger
  Logger::instanziate(Logger::LOG_VERBOSE);
  Logger::instance()->setLogLevel(m_Settings->get_Logger_LogLevel());

  // Plateform detail
  m_PlateformDetail = new PlateformDetail(this);

  // Application helper
  m_ApplicationHelper = new ApplicationHelper(m_Settings,
                                              m_PlateformDetail);
  m_ApplicationHelper->setDevelopmentMode(qMap_Arguments.value(CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE).toBool());
  m_ApplicationHelper->startupTimerStart();

  m_LocationManager = new LocationManager(m_Settings,
                                          m_PlateformDetail);
  m_PictureCacher = new PictureCacher(this);

  PictureRepository::instanziate();
  SpotRepository::instanziate(m_LocationManager);

  m_User = new User(m_Settings,
                    this);
  m_PictureUploader = new PictureUploader(this);

  m_QQmlApplicationEngine = new QQmlApplicationEngine();
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Application",       m_ApplicationHelper);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_PlateformDetail",   m_PlateformDetail);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_LocationManager",   m_LocationManager);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_PictureCacher",     m_PictureCacher);
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Logger",            Logger::instance());
  m_QQmlApplicationEngine->rootContext()->setContextProperty("hc_Settings",          m_Settings);

  m_QQmlApplicationEngine->rootContext()->setContextProperty("wa_User",              m_User           );
  m_QQmlApplicationEngine->rootContext()->setContextProperty("wa_PictureUploader",   m_PictureUploader);

  m_QQmlApplicationEngine->rootContext()->setContextProperty("re_PictureRepository", PictureRepository::instance());
  m_QQmlApplicationEngine->rootContext()->setContextProperty("re_SpotRepository",    SpotRepository::instance());

  qmlRegisterType<PicturesModel>   ("PicturesModel",    1, 0, "PicturesModel");
  qmlRegisterType<Picture>         ("Picture",          1, 0, "Picture");
  qmlRegisterType<SpotsModel>      ("SpotsModel",       1, 0, "SpotsModel");
  qmlRegisterType<Spot>            ("Spot",             1, 0, "Spot");

  QObject::connect(m_QQmlApplicationEngine,
                   SIGNAL(objectCreated(QObject*,QUrl)),
                   SLOT(slot_QmlApplicationEngine_objectCreated(QObject*,QUrl)));

  QTimer::singleShot(0.0,
                     this,
                     SLOT(slot_LoadQml()));

}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
  // Set and save settings
  m_Settings->set_Application_LastVersion(QApplication::applicationVersion());
  m_Settings->set_Logger_LogLevel(Logger::instance()->getLogLevel());
  m_Settings->set_Location_LastCoordinate(m_LocationManager->coordinate());
  m_Settings->sync();

  // Delete objects
  delete m_QQmlApplicationEngine;
  delete m_PictureUploader;
  delete m_User;
  delete m_PictureCacher;
  delete m_LocationManager;
  delete m_ApplicationHelper;
  delete m_PlateformDetail;
  delete m_Settings;

  // Destroy singletons
  Logger::destroy();
  SpotRepository::destroy();
  PictureRepository::destroy();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::slot_LoadQml()
{
  Logger::info("Load main.qml...");
  m_QQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::slot_QmlApplicationEngine_objectCreated(QObject *, QUrl)
{
  m_ApplicationHelper->startupTimerStop();

  // Try to login
  if(m_User->login() == false)
  {
    Logger::error(m_User->lastErrorText());
    return;
  }

  // Check newer version
  if(m_ApplicationHelper->checkCurrentClientVersion() == false)
  {
    return;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

QMap<QString, QVariant> Application::parseCommandLineArguments()
{
  QCommandLineParser qCommandLineParser;
  qCommandLineParser.setApplicationDescription(QApplication::applicationName());
  qCommandLineParser.addHelpOption();
  qCommandLineParser.addVersionOption();

  // Development mode
  QCommandLineOption qCommandLineOption_development(QStringList() << "d" << "development",
                                                    "Run in development mode.");
  qCommandLineParser.addOption(qCommandLineOption_development);

  qCommandLineParser.process(QApplication::arguments());

  QMap<QString, QVariant> qMap_Arguments;

  qMap_Arguments.insert(CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE, qCommandLineParser.isSet(qCommandLineOption_development));

  return qMap_Arguments;
}
