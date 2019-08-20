/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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
#include "../Settings/Secret.h"
#include "PlateformDetail.h"

// Library includes ------------------------
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QGeoCoordinate>
#include <QDesktopServices>
#include <QDebug>
#include <QPointF>
#include <QUrl>
#include <QTimer>

//-----------------------------------------------------------------------------------------------------------------------------

const int LocationManager::_CONST::GPS::UPDATE_ITERVAL_MS (4000);

//-----------------------------------------------------------------------------------------------------------------------------

LocationManager::LocationManager(Settings *settings,
                                 PlateformDetail *plateformDetail,
                                 QObject *parent)
  : QObject(parent),
    m_Settings(settings),
    m_PlateformDetail(plateformDetail),
    m_GeoPositionInfoSource(NULL),
    m_GeoPositionInfoSource_UpdatesCount(0),
    m_QGeoCoordinate()
{
  m_GeoPositionInfoSource = QGeoPositionInfoSource::createDefaultSource(this);

  if (m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(QString("LocationManager::%1() Invalid GeoPositionInfoSource").arg(__FUNCTION__));

    // Set position from last position
    setCurrentPosition(m_Settings->get_Location_LastCoordinate());
  }
  else
  {
    m_GeoPositionInfoSource->setUpdateInterval(_CONST::GPS::UPDATE_ITERVAL_MS);

    if(m_GeoPositionInfoSource->lastKnownPosition().isValid())
    {
      // Set position from last known position
      setCurrentPosition(m_GeoPositionInfoSource->lastKnownPosition().coordinate());
    }
    else
    {
      // Set position from last position settings
      setCurrentPosition(m_Settings->get_Location_LastCoordinate());
    }

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

void LocationManager::setCurrentPosition(const QGeoCoordinate &qGeoCoordinate)
{
  m_QGeoCoordinate = qGeoCoordinate;

  emit signal_Coordinate_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::setCurrentPosition(double latitude,
                                      double longitude)
{
  m_QGeoCoordinate.setLatitude (latitude);
  m_QGeoCoordinate.setLongitude(longitude);

  emit signal_Coordinate_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::suspendUpdates()
{
  Logger::info(QString("LocationManager::%1()").arg(__FUNCTION__));

  if(m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(tr("Invalid GeoPositionInfoSource"));
    return;
  }

  m_GeoPositionInfoSource->stopUpdates();
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::resumeUpdates()
{
  Logger::info(QString("LocationManager::%1()").arg(__FUNCTION__));

  if(m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(tr("Invalid GeoPositionInfoSource"));
    return;
  }

  if(m_GeoPositionInfoSource_UpdatesCount > 0)
  {
      QTimer::singleShot(0,
                         m_GeoPositionInfoSource,
                         SLOT(startUpdates()));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::startUpdates()
{
  Logger::info(QString("LocationManager::%1()").arg(__FUNCTION__));

  if(m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(QString("LocationManager::%1(): Invalid GeoPositionInfoSource").arg(__FUNCTION__));
    return;
  }

  if(m_GeoPositionInfoSource_UpdatesCount == 0)
  {
      QTimer::singleShot(0,
                         m_GeoPositionInfoSource,
                         SLOT(startUpdates()));
  }

  m_GeoPositionInfoSource_UpdatesCount++;
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::stopUdates()
{
  Logger::info(QString("LocationManager::%1()").arg(__FUNCTION__));
  if(m_GeoPositionInfoSource == NULL)
  {
    Logger::warning(QString("LocationManager::%1(): Invalid GeoPositionInfoSource").arg(__FUNCTION__));
    return;
  }

  if(m_GeoPositionInfoSource_UpdatesCount > 0)
  {
    m_GeoPositionInfoSource_UpdatesCount--;
  }

  if(m_GeoPositionInfoSource_UpdatesCount == 0)
      m_GeoPositionInfoSource->stopUpdates();
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

QByteArray LocationManager::mapboxAccessToken() const
{
  return CONST_MAPBOX_ACCESSTOKEN.toLatin1();
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

  Logger::trace(QString("LocationManager::%1() url: %2").arg(__FUNCTION__)
                                                        .arg(url));

  if(QDesktopServices::openUrl(QUrl(url)) == false)
    Logger::error(QString("LocationManager::%1(): Failed to open url '%2'").arg(QString(__FUNCTION__))
                                                                           .arg(url));
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_positionUpdated(QGeoPositionInfo geo_position_info)
{
  if(geo_position_info.isValid() == false)
  {
    Logger::error(QString("LocationManager::%1(): GeoPositionInfo is not valid").arg(__FUNCTION__));
    return;
  }

  // Set current position
  setCurrentPosition(geo_position_info.coordinate());
}

//-----------------------------------------------------------------------------------------------------------------------------

void LocationManager::slot_GeoPositionInfoSource_UpdateTimeout()
{
  Logger::warning(QString("LocationManager::%1()").arg(__FUNCTION__));
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


