/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "NearbySpotsModel.h"

// Projects includes -----------------------
#include "Spot.h"
#include "SpotRepository.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

NearbySpotsModel::NearbySpotsModel(QObject *parent)
  : QAbstractListModel(parent),
    m_QList_Spots(),
    m_RequestId(0)
{
  connect(SpotRepository::instance(),
          SIGNAL(signal_DataReady(int,
                                  bool)),
          SLOT(slot_SpotRepository_DataReady(int,
                                                bool)));
}

//-----------------------------------------------------------------------------------------------------------------------------

NearbySpotsModel::~NearbySpotsModel()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

int NearbySpotsModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_QList_Spots.count();
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant NearbySpotsModel::data(const QModelIndex &index,
                                int role) const
{
  return m_QList_Spots.at(index.row())->spotRole((Spot::SpotRoles)role);
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> NearbySpotsModel::roleNames() const
{
  return Spot::roleNames();
}

//-----------------------------------------------------------------------------------------------------------------------------

void NearbySpotsModel::setLocation(double latitude,
                                   double longitude,
                                   double maxDistance_km)
{
  m_RequestId = SpotRepository::instance()->getBy_Distance(latitude,
                                                          longitude,
                                                          maxDistance_km);
}

//-----------------------------------------------------------------------------------------------------------------------------

void NearbySpotsModel::slot_SpotRepository_DataReady(int requestId,
                                                     bool success)
{
  if(m_RequestId != requestId)
    return;

  if(success == false)
    return;
  // TODO error handling?

  beginResetModel();
  m_QList_Spots.clear();
  endResetModel();

  beginInsertRows(QModelIndex() , 0, SpotRepository::instance()->getSpots(m_RequestId).size()-1);
  m_QList_Spots = SpotRepository::instance()->getSpots(m_RequestId);
  endInsertRows();
}

//-----------------------------------------------------------------------------------------------------------------------------
