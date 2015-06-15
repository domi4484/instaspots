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
#include "WebApi.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

NearbySpotsModel::NearbySpotsModel(QObject *parent)
  : QAbstractListModel(parent),
    m_Command_GetNearbySpots()
{
  m_Command_GetNearbySpots.setAnswerType(WebApiCommand::JSON);
  m_Command_GetNearbySpots.setCommand(WebApi::C_GET_NEARBY_SPOTS);
  connect(&m_Command_GetNearbySpots,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandGetNearbySpots_Finished(const WebApiError &)));
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

void NearbySpotsModel::setLocation(float latitude,
                                   float longitude)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_LATITUDE,  QString::number(latitude)));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_LONGITUDE, QString::number(longitude)));

  // TODO check post return type
  m_Command_GetNearbySpots.postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void NearbySpotsModel::slot_CommandGetNearbySpots_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    return;
  }

  QScriptValue qScriptValue_Ids = m_Command_GetNearbySpots.resultProperty(WebApi::A_ARRAY_SPOTS);
  int length = qScriptValue_Ids.property("length").toInteger();

  beginResetModel();
  m_QList_Spots.clear();
  endResetModel();

  beginInsertRows(QModelIndex(), 0, length-1);

  for(int i = 0; i < length; i++)
  { 
    m_QList_Spots.append(new Spot(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_ID).toInteger(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_NAME).toString(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION).toString(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE).toNumber(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE).toNumber(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE).toNumber(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1).toString(),
                                  qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2).toString()));
  }

  endInsertRows();
}

//-----------------------------------------------------------------------------------------------------------------------------
