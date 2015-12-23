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

SpotsModel::SpotsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_QList_Spots(),
    m_RequestId(0)
{
    m_RequestId = SpotRepository::instance()->getNewRequestId();

    connect(SpotRepository::instance(),
            SIGNAL(signal_DataReady(int,
                                    bool)),
            SLOT(slot_SpotRepository_DataReady(int,
                                               bool)));
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

void SpotsModel::setLocation(double latitude, double longitude, double maxDistance_km)
{
    SpotRepository::instance()->getBy_Distance(m_RequestId,
                                               latitude,
                                               longitude,
                                               maxDistance_km);
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

  QAbstractItemModel::beginResetModel();
  m_QList_Spots.clear();
  QAbstractItemModel::endResetModel();

  QAbstractItemModel::beginInsertRows(QModelIndex() , 0, SpotRepository::instance()->getSpots(m_RequestId).size()-1);
  m_QList_Spots = SpotRepository::instance()->getSpots(m_RequestId);
  QAbstractItemModel::endInsertRows();

  countChanged(m_QList_Spots.count());
}

//-----------------------------------------------------------------------------------------------------------------------------


