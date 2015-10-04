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

// Qt includes -----------------------------
#include <QSettings>

class Settings : public QSettings
{
  Q_OBJECT

public:

  explicit Settings(QObject *parent = 0);
  virtual ~Settings();

  // Application settings
  static const QString APPLICATION_LAST_VERSION;
  static const QString APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT;

  // User settings
  static const QString USER_USERNAME;
  static const QString USER_PASSWORD;

  // Location settings
  static const QString LOCATION_LAST_LATITUDE;
  static const QString LOCATION_LAST_LONGITUDE;
};

#endif // SETTINGS_H
