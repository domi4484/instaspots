/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------

// Project includes ------------------------
#include "HelperClasses/Application.h"
#include "HelperClasses/Logger.h"

// Qt includes -----------------------------

//-----------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    Application application(argc,
                            argv);

    Logger::info("Application exec.");
    int exitCode = application.exec();
    Logger::info("Application closing...");

    return exitCode;
}

