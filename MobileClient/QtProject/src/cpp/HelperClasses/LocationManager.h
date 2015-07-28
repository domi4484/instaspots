/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      28.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

// Qt includes -----------------------------
#include <QObject>
#include <QGeoPositionInfoSource>

// Forward declarations --------------------
class Settings;

class LocationManager : public QObject
{
  Q_OBJECT
public:
  explicit LocationManager(Settings *settings,
                           QObject *parent = 0);
  ~LocationManager();

signals:

  void update(bool timeout);

  void signal_RequestLocation();

public slots:

  void setFakePosition(double latitude,
                       double longitude);

  void requestLocation();

  bool isValid();

  double latitude();
  double longitude();

  QString googleMapLocationPicture(int width,
                                   int height);

private slots:
  void slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info);
  void slot_GeoPositionInfoSource_UpdateTimeout();
  void slot_GeoPositionInfoSource_error(QGeoPositionInfoSource::Error error);
private:

  // Link to Settings
  Settings *m_Settings;

//  QGeoPositionInfoSource *m_GeoPositionInfoSource;

  bool m_Valid;

  double m_Latitude;
  double m_Longitude;

};

#endif // LOCATIONMANAGER_H
