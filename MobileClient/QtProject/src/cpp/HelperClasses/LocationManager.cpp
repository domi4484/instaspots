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
#include "Logger.h"

// Qt includes -----------------------------
#include <QGeoCoordinate>

LocationManager::LocationManager(QObject *parent)
  : QObject(parent),
    m_GeoPositionInfoSource(NULL),
    m_Latitude(0.0),
    m_Longitude(0.0)
{
  m_GeoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);

  if (m_GeoPositionInfoSource != NULL)
  {
    m_GeoPositionInfoSource->setUpdateInterval(3000);
    connect(m_GeoPositionInfoSource,
            SIGNAL(positionUpdated(QGeoPositionInfo)),
            SLOT(slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo)));
    connect(m_GeoPositionInfoSource,
            SIGNAL(updateTimeout()),
            SLOT(slot_GeoPositionInfoSource_UpdateTimeout()));
    connect(m_GeoPositionInfoSource,
            SIGNAL(error(QGeoPositionInfoSource::Error)),
            SLOT(slot_GeoPositionInfoSource_error(QGeoPositionInfoSource::Error)));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

LocationManager::~LocationManager()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::requestLocation()
{
  if(m_GeoPositionInfoSource == NULL)
    return;

  m_GeoPositionInfoSource->requestUpdate();
}

//-----------------------------------------------------------------------------------------------------------------------------

double LocationManager::latitude()
{
  return m_Latitude;
}

double LocationManager::longitude()
{
  return m_Longitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info)
{
  m_Latitude  = geo_position_info.coordinate().latitude();
  m_Longitude = geo_position_info.coordinate().longitude();

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
