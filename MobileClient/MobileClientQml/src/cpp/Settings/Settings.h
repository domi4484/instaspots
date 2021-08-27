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

#ifndef SETTINGS_H
#define SETTINGS_H

// Library includes ------------------------
#include <HelperClasses/Logger.h>

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

  // Server settings

  // User settings
  static const QString USER_TOKEN;
  static const QString USER_USERNAME;
  static const QString USER_ID;

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
  QString get_Application_LastVersion () const;
  void    set_Application_LastVersion (const QString &lastVersion);
  bool    get_Application_NewerVersionAvailableGotIt () const;
  void    set_Application_NewerVersionAvailableGotIt (bool newerVersionAvailableGotIt);

  // Logger settings
  Logger::LOG_LEVEL get_Logger_LogLevel () const;
  void              set_Logger_LogLevel (Logger::LOG_LEVEL logLevel);

  // User settings
  QString get_User_Token() const;
  void set_User_Token(const QString &token);
  int get_User_Id() const;
  void set_User_Id(int id);
  QString get_User_Username() const;
  void set_User_Username(const QString &username);

  // Location settings
  QGeoCoordinate  get_Location_LastCoordinate () const;
  void            set_Location_LastCoordinate (const QGeoCoordinate &coordinate);

  // One time help settings
  bool get_HelpGotIt_UploadNewPictureSecretSpot() const;
  void set_HelpGotIt_UploadNewPictureSecretSpot(bool gotIt);

  // Special functions
  void resetGotItSettings();
};

#endif // SETTINGS_H
