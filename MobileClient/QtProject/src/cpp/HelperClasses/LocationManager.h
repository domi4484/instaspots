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

class LocationManager : public QObject
{
  Q_OBJECT
public:
  explicit LocationManager(QObject *parent = 0);
  ~LocationManager();

signals:

  void update(bool timeout);

public slots:

  void requestLocation();

  double latitude();
  double longitude();

private slots:
  void slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info);
  void slot_GeoPositionInfoSource_UpdateTimeout();
  void slot_GeoPositionInfoSource_error(QGeoPositionInfoSource::Error error);
private:

  QGeoPositionInfoSource *m_GeoPositionInfoSource;

  double m_Latitude;
  double m_Longitude;

};

#endif // LOCATIONMANAGER_H
