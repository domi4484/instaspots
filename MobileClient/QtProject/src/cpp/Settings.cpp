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
const QString Settings::APPLICATION_LAST_VERSION                   ("Application_LastVersion");
const QString Settings::APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT ("Application_NewerVersionAvailableGotIt");

// Logger settings
const QString Settings::LOGGER_LOG_LEVEL ("Logger_LogLevel");

// User settings
const QString Settings::USER_USERNAME ("User_Username");
const QString Settings::USER_PASSWORD ("User_Password");

// Location settings
const QString Settings::LOCATION_LAST_LATITUDE  ("Location_LastLatitude");
const QString Settings::LOCATION_LAST_LONGITUDE ("Location_LastLongitude");

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Settings(QObject *parent) :
  QSettings(parent)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

Settings::~Settings()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_Application_LastVersion()
{
  return QSettings::value(APPLICATION_LAST_VERSION, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Application_LastVersion(const QString &lastVersion)
{
  QSettings::setValue(APPLICATION_LAST_VERSION, lastVersion);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Settings::get_Application_NewerVersionAvailableGotIt()
{
  return QSettings::value(APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, false).toBool();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Application_NewerVersionAvailableGotIt(bool newerVersionAvailableGotIt)
{
  QSettings::setValue(APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, newerVersionAvailableGotIt);
}

//-----------------------------------------------------------------------------------------------------------------------------

Logger::LOG_LEVEL Settings::get_Logger_LogLevel()
{
  return (Logger::LOG_LEVEL) QSettings::value(LOGGER_LOG_LEVEL, Logger::LOG_INFO).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Logger_LogLevel(Logger::LOG_LEVEL logLevel)
{
  QSettings::setValue(LOGGER_LOG_LEVEL, logLevel);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_User_Username()
{
  return QSettings::value(USER_USERNAME, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_User_Username(const QString &username)
{
  QSettings::setValue(USER_USERNAME, username);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_User_Password()
{
  return QSettings::value(USER_PASSWORD, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_User_Password(const QString &password)
{
  QSettings::setValue(USER_PASSWORD, password);
}

//-----------------------------------------------------------------------------------------------------------------------------

double Settings::get_Location_LastLatitude()
{
  return QSettings::value(LOCATION_LAST_LATITUDE, 0.0).toDouble();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Location_LastLatitude(double latitude)
{
  QSettings::setValue(LOCATION_LAST_LATITUDE, latitude);
}

//-----------------------------------------------------------------------------------------------------------------------------

double Settings::get_Location_LastLongitude()
{
  return QSettings::value(LOCATION_LAST_LONGITUDE, 0.0).toDouble();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Location_LastLongitude(double longitude)
{
  QSettings::setValue(LOCATION_LAST_LONGITUDE, longitude);
}


