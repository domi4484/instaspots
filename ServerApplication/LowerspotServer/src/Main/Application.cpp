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

// Project includes ------------------------
#include "../Settings/Settings.h"
#include "CommandReceiver/ApplicationCommandReceiver.h"

// Library includes ------------------------
#include <HelperClasses/Exception.h>
#include <HelperClasses/Logger.h>
#include <TcpIp/TcpIpServer.h>

// Qt includes -----------------------------
#include <QDir>
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Application::_CONST::LOGGER::DIRECTORY("Logs");
const QString Application::_CONST::SETTINGS::DIRECTORY("Settings");
const QString Application::_CONST::SETTINGS::FILENAME("Settings");

//-----------------------------------------------------------------------------------------------------------------------------

Application::Application(int argc, char *argv[])
  : QCoreApplication(argc, argv)
  , m_Settings(nullptr)
  , m_ServerApplicationCommandReceiver(nullptr)
  , m_TcpIpServer(nullptr)
{
  // Application informations
  QCoreApplication::setOrganizationName   ("Lowerspot");
  QCoreApplication::setOrganizationDomain ("lowerspot.com");
  QCoreApplication::setApplicationName    ("LowerspotServer");
  QCoreApplication::setApplicationVersion ("V0.2.0");

  // startupApplication Logger
  startupApplication_Logger();

  Logger::info(QString("Starting %1 %2")
                      .arg(QCoreApplication::applicationName())
                      .arg(QCoreApplication::applicationVersion()));

  // startupApplication Settings
  startupApplication_Settings();

  // startupApplication ServerApplicationCommandReceiver
  startupApplication_CommandReceiver();

  // startupApplication Tcp/Ip Server
  startupApplication_TcpIpServer();
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
  delete m_TcpIpServer;
  delete m_ServerApplicationCommandReceiver;
  delete m_Settings;

  Logger::destroy();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::startupApplication_Logger()
{
  // Create logs directory if not existing
  QDir qDir;
  if(qDir.exists(_CONST::LOGGER::DIRECTORY) == false)
  {
    if(qDir.mkdir(_CONST::LOGGER::DIRECTORY) == false)
      qDebug() << QString("Can't create directory '%1' in '%2'")
                         .arg(qDir.path())
                         .arg(_CONST::LOGGER::DIRECTORY);
  }

  // Logger
  Logger::instanziate(Logger::LOG_DEBUG);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::startupApplication_Settings()
{
  // Create setting directory if not existing
  QDir qDir;
  if(qDir.exists(_CONST::SETTINGS::DIRECTORY) == false)
  {
    if(qDir.mkdir(_CONST::SETTINGS::DIRECTORY) == false)
      Logger::error(QString("Can't create directory '%1' in '%2'")
                           .arg(qDir.path())
                           .arg(_CONST::SETTINGS::DIRECTORY));
  }

  m_Settings = new Settings();

  // Settings fileinfo
  QFileInfo qFileInfo_Settings;
  {
    QDir qDir_Settings = qDir;
    qDir_Settings.cd(_CONST::SETTINGS::DIRECTORY);
    qFileInfo_Settings.setFile(qDir_Settings,
                               _CONST::SETTINGS::FILENAME
                               + Settings::_CONST::FILENAME_EXTENSION);
  }

  try
  {
    if(qFileInfo_Settings.exists())
      m_Settings->Load(qFileInfo_Settings);
    else
    {
      m_Settings->Write(qFileInfo_Settings.filePath());
      m_Settings->Load(qFileInfo_Settings);
    }
  }
  catch (const Exception &exception)
  {
    Logger::error(QString("Error loading settings: %1").arg(exception.GetText()));
  }

  // Set loglevel
  Logger::instance()->setLogLevel((Logger::LOG_LEVEL)m_Settings->Get_Values_Logger_LogLevel());
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::startupApplication_CommandReceiver()
{
  m_ServerApplicationCommandReceiver = new ServerApplicationCommandReceiver();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::startupApplication_TcpIpServer()
{
  m_TcpIpServer = new TcpIpServer(m_ServerApplicationCommandReceiver,
                                  this);

  try
  {
    m_TcpIpServer->StartListening(m_Settings->Get_Values_TcpIpServer_Port());
  }
  catch (const Exception &exception)
  {
    Logger::error(QString("Error starting Tcp/Ip server: %1").arg(exception.GetText()));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------


