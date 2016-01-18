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

// Files includes --------------------------
#include "LocationManager.h"

// Project includes ------------------------
#include "../Settings.h"
#include "Logger.h"

// Qt includes -----------------------------
#include <QGeoCoordinate>
#include <QDesktopServices>
#include <QPointF>
#include <QUrl>

LocationManager::LocationManager(Settings *settings,
                                 QObject *parent)
  : QObject(parent),
    m_Settings(settings),
//    m_GeoPositionInfoSource(NULL),
    m_Valid(false),
    m_Latitude(0.0),
    m_Longitude(0.0)
{
//  m_GeoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);

//  if (m_GeoPositionInfoSource == NULL)
//  {
//    Logger::warning(tr("Invalid GeoPositionInfoSource"));
//  }
//  else
//  {
//    m_GeoPositionInfoSource->setUpdateInterval(3000);
//    connect(m_GeoPositionInfoSource,
//            SIGNAL(positionUpdated(QGeoPositionInfo)),
//            SLOT(slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo)));
//    connect(m_GeoPositionInfoSource,
//            SIGNAL(updateTimeout()),
//            SLOT(slot_GeoPositionInfoSource_UpdateTimeout()));
//    connect(m_GeoPositionInfoSource,
//            SIGNAL(error(QGeoPositionInfoSource::Error)),
//            SLOT(slot_GeoPositionInfoSource_error(QGeoPositionInfoSource::Error)));
//  }

  m_Latitude  = m_Settings->get_Location_LastLatitude();
  m_Longitude = m_Settings->get_Location_LastLongitude();
}

//-----------------------------------------------------------------------------------------------------------------------------

LocationManager::~LocationManager()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::setFakePosition(double latitude,
                                      double longitude)
{
  m_Valid = true;

  m_Latitude  = latitude;
  m_Longitude = longitude;

  emit update(false);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::requestLocation()
{
  emit signal_RequestLocation();

//  if(m_GeoPositionInfoSource == NULL)
//  {
//    Logger::warning(tr("Invalid GeoPositionInfoSource"));
//    return;
//  }

//  Logger::debug(tr("LocationManager::requestLocation()"));
//  m_GeoPositionInfoSource->requestUpdate();
}

//-----------------------------------------------------------------------------------------------------------------------------

bool LocationManager::isValid()
{
  return m_Valid;
}

//-----------------------------------------------------------------------------------------------------------------------------

double LocationManager::latitude()
{
  return m_Latitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

double LocationManager::longitude()
{
  return m_Longitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

qreal LocationManager::computeDistance(const QPointF &point1,
                                       const QPointF &point2)
{
  QGeoCoordinate qGeoCoordinate1(point1.x(), point1.y());
  QGeoCoordinate qGeoCoordinate2(point2.x(), point2.y());

  return qGeoCoordinate1.distanceTo(qGeoCoordinate2) / 1000.0;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString LocationManager::googleMapLocationPicture(int width,
                                                  int height)
{
  return QString("https://maps.googleapis.com/maps/api/staticmap?center=%1,%2&zoom=17&size=%3x%4&maptype=roadmap&markers=color:0x7FB5BE%7C%1,%2")
                 .arg(m_Latitude)
                 .arg(m_Longitude)
                 .arg(width)
                 .arg(height);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::openLocationOnNativeMapsApp(double latitude,
                                                  double longitude,
                                                  const QString &label)
{
  QString url = QString("geo:%1,%2").arg(latitude)
                                    .arg(longitude);
  if(label.isEmpty() == false)
  {
    url.append(QString("?q=%1,%2(%3)").arg(latitude)
                                      .arg(longitude)
                                      .arg(label));
  }

  if(QDesktopServices::openUrl(QUrl(url)) == false)
    Logger::error(QString("%1: Failed to open url '%2'").arg(QString(__FUNCTION__))
                                                        .arg(url));
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info)
{
  m_Valid = true;

  m_Latitude  = geo_position_info.coordinate().latitude();
  m_Longitude = geo_position_info.coordinate().longitude();

  m_Settings->set_Location_LastLatitude(m_Latitude);
  m_Settings->set_Location_LastLongitude(m_Longitude);

  emit update(false);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_UpdateTimeout()
{
  Logger::warning(QString("LocationManager updateTimeout"));

  emit update(true);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_error(QGeoPositionInfoSource::Error error)
{
  switch (error)
  {
  case QGeoPositionInfoSource::AccessError:
    Logger::error("QGeoPositionInfoSource::AccessError");
  break;
  case QGeoPositionInfoSource::ClosedError:
    Logger::error("QGeoPositionInfoSource::ClosedError");
  break;
  case QGeoPositionInfoSource::NoError:
    Logger::error("QGeoPositionInfoSource::NoError");
  break;
  case QGeoPositionInfoSource::UnknownSourceError:
    Logger::error("QGeoPositionInfoSource::UnknownSourceError");
  break;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------


