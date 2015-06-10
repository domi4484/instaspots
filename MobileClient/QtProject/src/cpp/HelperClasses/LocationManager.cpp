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

LocationManager::LocationManager(Settings *settings,
                                 QObject *parent)
  : QObject(parent),
    m_Settings(settings),
    m_GeoPositionInfoSource(NULL),
    m_Valid(false),
    m_Latitude(0.0),
    m_Longitude(0.0)
{
  m_GeoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);

  if (m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(tr("Invalid GeoPositionInfoSource"));
  }
  else
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

  m_Latitude  = m_Settings->value(Settings::LOCATION_LAST_LATITUDE,  0.0).toDouble();
  m_Longitude = m_Settings->value(Settings::LOCATION_LAST_LONGITUDE, 0.0).toDouble();
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

  m_Latitude = latitude;
  m_Longitude = longitude;

  emit update(false);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::requestLocation()
{
  if(m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(tr("Invalid GeoPositionInfoSource"));
    return;
  }

  Logger::debug(tr("LocationManager::requestLocation()"));
  m_GeoPositionInfoSource->requestUpdate();
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

QString LocationManager::googleMapLocationPicture(int width,
                                                  int height)
{
  return QString("https://maps.googleapis.com/maps/api/staticmap?center=%1,%2&zoom=17&size=%3x%4&maptype=roadmap").arg(m_Latitude)
                                                                                                                 .arg(m_Longitude)
                                                                                                                 .arg(width)
                                                                                                                 .arg(height);
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info)
{
  m_Valid = true;

  m_Latitude  = geo_position_info.coordinate().latitude();
  m_Longitude = geo_position_info.coordinate().longitude();

  m_Settings->setValue(Settings::LOCATION_LAST_LATITUDE,  m_Latitude);
  m_Settings->setValue(Settings::LOCATION_LAST_LONGITUDE, m_Longitude);

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
