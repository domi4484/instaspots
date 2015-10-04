/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      26.05.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "Settings.h"

//-----------------------------------------------------------------------------------------------------------------------------

// Application settings
const QString Settings::APPLICATION_LAST_VERSION                   ("application_last_version");
const QString Settings::APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT ("application_newer_version_available_got_it");

// User settings
const QString Settings::USER_USERNAME("username");
const QString Settings::USER_PASSWORD("password");

// Location settings
const QString Settings::LOCATION_LAST_LATITUDE ("last_latitude");
const QString Settings::LOCATION_LAST_LONGITUDE("last_longitude");

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Settings(QObject *parent) :
  QSettings(parent)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

Settings::~Settings()
{

}

