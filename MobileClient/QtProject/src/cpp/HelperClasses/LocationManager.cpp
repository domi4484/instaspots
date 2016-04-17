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
#include "../Settings/Settings.h"
#include "PlateformDetail.h"
#include "Logger.h"

// Qt includes -----------------------------
#include <QGeoCoordinate>
#include <QDesktopServices>
#include <QPointF>
#include <QUrl>

LocationManager::LocationManager(Settings *settings,
                                 PlateformDetail *plateformDetail,
                                 QObject *parent)
  : QObject(parent),
    m_Settings(settings),
    m_PlateformDetail(plateformDetail),
//    m_GeoPositionInfoSource(NULL),
    m_Valid(false),
    m_QGeoCoordinate()
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

  m_QGeoCoordinate = m_Settings->get_Location_LastCoordinate();
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

  m_QGeoCoordinate.setLatitude (latitude);
  m_QGeoCoordinate.setLongitude(longitude);

  emit update(false);
  emit signal_Coordinate_changed();
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
  return m_QGeoCoordinate.latitude();
}

//-----------------------------------------------------------------------------------------------------------------------------

double LocationManager::longitude()
{
  return m_QGeoCoordinate.longitude();
}

//-----------------------------------------------------------------------------------------------------------------------------

QGeoCoordinate LocationManager::coordinate()
{
  return m_QGeoCoordinate;
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

void LocationManager::openLocationOnNativeMapsApp(double latitude,
                                                  double longitude,
                                                  const QString &label)
{
  QString url = QString("geo:%1,%2").arg(latitude)
                                    .arg(longitude);


  if(m_PlateformDetail->isIOS())
  {
    url = QString("http://maps.google.com/maps/@%1,%2").arg(latitude)
                                                       .arg(longitude);
  }

  if(label.isEmpty() == false)
  {
    url.append(QString("?q=%1,%2(%3)").arg(latitude)
                                      .arg(longitude)
                                      .arg(label));
  }

  Logger::trace(QString("LocationManager::openLocationOnNativeMapsApp url: %1").arg(url));

  if(QDesktopServices::openUrl(QUrl(url)) == false)
    Logger::error(QString("%1: Failed to open url '%2'").arg(QString(__FUNCTION__))
                                                        .arg(url));
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info)
{
  m_Valid = true;

  m_QGeoCoordinate = geo_position_info.coordinate();

  emit signal_Coordinate_changed();
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


