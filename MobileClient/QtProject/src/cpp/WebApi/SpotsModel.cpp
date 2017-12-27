/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.10.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "SpotsModel.h"

// Projects includes -----------------------
#include "../HelperClasses/Logger.h"
#include "Spot.h"
#include "SpotRepository.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const double SpotsModel::_CONST::MIN_DISTANCE_BETWEEN_UPDATES_KM (0.015);
const int    SpotsModel::_CONST::UPDATE_MODEL_DELAY_MS           (500);

//-----------------------------------------------------------------------------------------------------------------------------

SpotsModel::SpotsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_QList_Spots(),
    m_RequestId(0),
    m_QGeoRectangle_VisibleRegion(),
    m_QTimer_UpdateModel()
{
    m_RequestId = SpotRepository::instance()->getNewRequestId();

    m_QTimer_UpdateModel.setSingleShot(true);

    // Signals/slots
    connect(SpotRepository::instance(),
            SIGNAL(signal_DataReady(int,
                                    bool)),
            SLOT(slot_SpotRepository_DataReady(int,
                                               bool)));
    connect(SpotRepository::instance(),
            SIGNAL(signal_SpotRemoved(Spot *)),
            SLOT(slot_SpotRepository_SpotRemoved(Spot *)));

    connect(&m_QTimer_UpdateModel,
            SIGNAL(timeout()),
            SLOT(slot_QTimer_UpdateModel_timeout()));
}

//-----------------------------------------------------------------------------------------------------------------------------

SpotsModel::~SpotsModel()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

int SpotsModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_QList_Spots.count();
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant SpotsModel::data(const QModelIndex &index,
                          int role) const
{
  return m_QList_Spots.at(index.row())->spotRole((Spot::SpotRoles)role);
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> SpotsModel::roleNames() const
{
    return Spot::roleNames();
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::clear()
{
    beginResetModel();
    m_QList_Spots.clear();
    endResetModel();
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::setUserId(int id)
{
    SpotRepository::instance()->getBy_UserId(m_RequestId,
                                             id);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::getBy_Distance(const QGeoCoordinate &qGeoCoordinate,
                                double maxDistance_m)
{
  double maxDistance_Degree = maxDistance_m / 111111.0;
  m_QGeoRectangle_VisibleRegion.setCenter(qGeoCoordinate);
  m_QGeoRectangle_VisibleRegion.setHeight(maxDistance_Degree);
  m_QGeoRectangle_VisibleRegion.setWidth(maxDistance_Degree);

  // Update direct
  slot_QTimer_UpdateModel_timeout();
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::updateBy_Location(const QGeoCoordinate &qGeoCoordinate)
{
  if(m_QGeoRectangle_VisibleRegion.center().distanceTo(qGeoCoordinate) < _CONST::MIN_DISTANCE_BETWEEN_UPDATES_KM * 1000.0)
  {
    return;
  }

  m_QGeoRectangle_VisibleRegion.setCenter(qGeoCoordinate);

  m_QTimer_UpdateModel.start(_CONST::UPDATE_MODEL_DELAY_MS);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::updateBy_VisibleRegion(const QGeoRectangle &qGeoRectangle)
{
  m_QGeoRectangle_VisibleRegion = qGeoRectangle;

  m_QTimer_UpdateModel.start(_CONST::UPDATE_MODEL_DELAY_MS);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::slot_SpotRepository_DataReady(int requestId,
                                               bool success)
{
  if(m_RequestId != requestId)
    return;

  if(success == false)
  {
    Logger::error(QString("slot_SpotRepository_DataReady: success = %1").arg(success));
    return;
  }

  if(m_QList_Spots != SpotRepository::instance()->getSpots(m_RequestId))
  {
    QAbstractItemModel::beginResetModel();
    m_QList_Spots.clear();
    QAbstractItemModel::endResetModel();

    int newCount =SpotRepository::instance()->getSpots(m_RequestId).size();
    if(newCount > 0)
    {
      QAbstractItemModel::beginInsertRows(QModelIndex() , 0, SpotRepository::instance()->getSpots(m_RequestId).size()-1);
      m_QList_Spots = SpotRepository::instance()->getSpots(m_RequestId);
      QAbstractItemModel::endInsertRows();
    }
    countChanged(m_QList_Spots.count());
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::slot_SpotRepository_SpotRemoved(Spot *spot)
{
  if(m_QList_Spots.contains(spot) == false)
  {
    return;
  }

  int index = m_QList_Spots.indexOf(spot);

  QAbstractItemModel::beginRemoveRows(QModelIndex(),
                                      index,
                                      index);
  m_QList_Spots.removeOne(spot);
  QAbstractItemModel::endRemoveRows();

  countChanged(m_QList_Spots.count());
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotsModel::slot_QTimer_UpdateModel_timeout()
{
  double latitude   = m_QGeoRectangle_VisibleRegion.center().latitude();
  double longitude  = m_QGeoRectangle_VisibleRegion.center().longitude();
  double distance_m = m_QGeoRectangle_VisibleRegion.topLeft().distanceTo(m_QGeoRectangle_VisibleRegion.bottomRight()) / 2.0;

  Logger::info(QString("SpotsModel::%1() coordinate=%2;%3, distance_m=%4)").arg(__FUNCTION__)
                                                                           .arg(latitude)
                                                                           .arg(longitude)
                                                                           .arg(distance_m / 1000));

  SpotRepository::instance()->getBy_Distance(m_RequestId,
                                             latitude,
                                             longitude,
                                             distance_m);
}

//-----------------------------------------------------------------------------------------------------------------------------


