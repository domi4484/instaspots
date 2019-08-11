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

// Qt includes -----------------------------
#include <QDir>
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Application::_CONST::SETTINGS::DIRECTORY("Settings");

//-----------------------------------------------------------------------------------------------------------------------------

Application::Application(int argc, char *argv[]) :
  QCoreApplication(argc, argv)
{
  // Application informations
  QCoreApplication::setOrganizationName   ("Lowerspot");
  QCoreApplication::setOrganizationDomain ("lowerspot.com");
  QCoreApplication::setApplicationName    ("LowerspotServer");
  QCoreApplication::setApplicationVersion ("V0.0.1");

  // startupApplication Settings
  startupApplication_Settings();
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void Application::startupApplication_Settings()
{
  // Create setting directory if not existing
  QDir qDir;
  if(qDir.exists(_CONST::SETTINGS::DIRECTORY) == false)
  {
    if(qDir.mkdir(_CONST::SETTINGS::DIRECTORY) == false)
      qDebug() << QString("Can't create directory '%1' in '%2'")
                         .arg(qDir.path())
                         .arg(_CONST::SETTINGS::DIRECTORY);
  }


}

//-----------------------------------------------------------------------------------------------------------------------------
