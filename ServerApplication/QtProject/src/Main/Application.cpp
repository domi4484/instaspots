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

//-----------------------------------------------------------------------------------------------------------------------------

Application::Application(int argc, char *argv[]) :
  QCoreApplication(argc, argv)
{
  // Application informations
  QCoreApplication::setOrganizationName   ("Lowerspot");
  QCoreApplication::setOrganizationDomain ("lowerspot.com");
  QCoreApplication::setApplicationName    ("LowerspotServer");
  QCoreApplication::setApplicationVersion ("V0.0.1");
}

//-----------------------------------------------------------------------------------------------------------------------------

Application::~Application()
{
}

//-----------------------------------------------------------------------------------------------------------------------------
