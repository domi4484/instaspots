/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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
const QString Settings::USER_TOKEN  ("User_Token");
const QString Settings::USER_USERNAME  ("User_Username");
const QString Settings::USER_ID ("User_Id");

// Location settings
const QString Settings::LOCATION_LAST_LATITUDE  ("Location_LastLatitude");
const QString Settings::LOCATION_LAST_LONGITUDE ("Location_LastLongitude");

// One Time Help settings
const QString Settings::HELP_GOT_IT_UPLOAD_NEW_PICTURE_SECRET_SPOT ("HelpGotIt_UploadNewPictureSecretSpot");

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Settings(QObject *parent)
  : QSettings(parent)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Settings::~Settings()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_Application_LastVersion() const
{
  return QSettings::value(APPLICATION_LAST_VERSION, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Application_LastVersion(const QString &lastVersion)
{
  QSettings::setValue(APPLICATION_LAST_VERSION, lastVersion);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Settings::get_Application_NewerVersionAvailableGotIt() const
{
  return QSettings::value(APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, true).toBool();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Application_NewerVersionAvailableGotIt(bool newerVersionAvailableGotIt)
{
  QSettings::setValue(APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, newerVersionAvailableGotIt);
}

//-----------------------------------------------------------------------------------------------------------------------------

Logger::LOG_LEVEL Settings::get_Logger_LogLevel() const
{
  return (Logger::LOG_LEVEL) QSettings::value(LOGGER_LOG_LEVEL, Logger::LOG_DEBUG).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Logger_LogLevel(Logger::LOG_LEVEL logLevel)
{
  QSettings::setValue(LOGGER_LOG_LEVEL, logLevel);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_User_Token() const
{
  return QSettings::value(USER_TOKEN, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_User_Token(const QString &token)
{
  QSettings::setValue(USER_TOKEN, token);
}

//-----------------------------------------------------------------------------------------------------------------------------

int Settings::get_User_Id() const
{
  return QSettings::value(USER_ID, -1).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_User_Id(int id)
{
    QSettings::setValue(USER_ID, id);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::get_User_Username() const
{
  return QSettings::value(USER_USERNAME, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_User_Username(const QString &username)
{
  QSettings::setValue(USER_USERNAME, username);
}

//-----------------------------------------------------------------------------------------------------------------------------

QGeoCoordinate Settings::get_Location_LastCoordinate() const
{
  return QGeoCoordinate(QSettings::value(LOCATION_LAST_LATITUDE,  46.528742).toDouble(),
                        QSettings::value(LOCATION_LAST_LONGITUDE, 8.624216 ).toDouble());
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_Location_LastCoordinate(const QGeoCoordinate &coordinate)
{
  QSettings::setValue(LOCATION_LAST_LATITUDE,  coordinate.latitude());
  QSettings::setValue(LOCATION_LAST_LONGITUDE, coordinate.longitude());
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Settings::get_HelpGotIt_UploadNewPictureSecretSpot() const
{
  return QSettings::value(HELP_GOT_IT_UPLOAD_NEW_PICTURE_SECRET_SPOT).toBool();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::set_HelpGotIt_UploadNewPictureSecretSpot(bool gotIt)
{
  QSettings::setValue(HELP_GOT_IT_UPLOAD_NEW_PICTURE_SECRET_SPOT, gotIt);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::resetGotItSettings()
{
  set_Application_NewerVersionAvailableGotIt(false);

  set_HelpGotIt_UploadNewPictureSecretSpot(false);
}

