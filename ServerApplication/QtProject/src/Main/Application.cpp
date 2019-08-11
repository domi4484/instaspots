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
#include "../HelperClasses/Exception.h"
#include "../HelperClasses/Logger.h"

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
{
  // Application informations
  QCoreApplication::setOrganizationName   ("Lowerspot");
  QCoreApplication::setOrganizationDomain ("lowerspot.com");
  QCoreApplication::setApplicationName    ("LowerspotServer");
  QCoreApplication::setApplicationVersion ("V0.0.1");

  // startupApplication Logger
  startupApplication_Logger();

  Logger::info(QString("Starting %1 %2")
                      .arg(QCoreApplication::applicationName())
                      .arg(QCoreApplication::applicationVersion()));

  // startupApplication Settings
  startupApplication_Settings();
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
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
  } catch (const Exception &exception)
  {
    Logger::error(QString("Error loading settings: %1").arg(exception.GetText()));
  }

  // Set loglevel
  Logger::instance()->setLogLevel((Logger::LOG_LEVEL)m_Settings->Get_Values_Logger_LogLevel());
}

//-----------------------------------------------------------------------------------------------------------------------------


