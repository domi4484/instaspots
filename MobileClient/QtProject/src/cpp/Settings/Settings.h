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

#ifndef SETTINGS_H
#define SETTINGS_H

// Project includes ------------------------
#include "../HelperClasses/Logger.h"

// External includes -----------------------
#include "../External/SimpleCrypt.h"

// Qt includes -----------------------------
#include <QGeoCoordinate>
#include <QSettings>

class Settings : public QSettings
{
  Q_OBJECT

public:

  // Application settings
  static const QString APPLICATION_LAST_VERSION;
  static const QString APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT;

  // Logger settings
  static const QString LOGGER_LOG_LEVEL;

  // User settings
  static const QString USER_USERNAME;
  static const QString USER_PASSWORD;
  static const QString USER_LOGGED_IN;

  // Location settings
  static const QString LOCATION_LAST_LATITUDE;
  static const QString LOCATION_LAST_LONGITUDE;

  // One Time Help settings
  static const QString HELP_GOT_IT_UPLOAD_NEW_PICTURE_SECRET_SPOT;

  // Constructor
  explicit Settings(QObject *parent = 0);
  virtual ~Settings();

public slots:

  // Application settings
  QString get_Application_LastVersion ();
  void    set_Application_LastVersion (const QString &lastVersion);
  bool    get_Application_NewerVersionAvailableGotIt ();
  void    set_Application_NewerVersionAvailableGotIt (bool newerVersionAvailableGotIt);

  // Logger settings
  Logger::LOG_LEVEL get_Logger_LogLevel ();
  void              set_Logger_LogLevel (Logger::LOG_LEVEL logLevel);

  // User settings
  QString get_User_Username ();
  void    set_User_Username (const QString &username);
  QString get_User_Password ();
  void    set_User_Password (const QString &password);
  bool    get_User_LoggedIn ();
  void    set_User_LoggedIn (bool loggedIn);

  // Location settings
  QGeoCoordinate  get_Location_LastCoordinate () const;
  void            set_Location_LastCoordinate (const QGeoCoordinate &coordinate);

  // One time help settings
  bool get_HelpGotIt_UploadNewPictureSecretSpot();
  void set_HelpGotIt_UploadNewPictureSecretSpot(bool gotIt);

  // Special functions
  void resetGotItSettings();

private:

  SimpleCrypt m_SimpleCrypt;
};

#endif // SETTINGS_H
