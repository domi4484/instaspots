/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.06.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "SpotRepository.h"

// Project includes ------------------------
#include "Spot.h"
#include "WebApi.h"
#include "WebApiCommand.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QJsonArray>

//-----------------------------------------------------------------------------------------------------------------------------

const char* SpotRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

SpotRepository *SpotRepository::s_SpotRepository = NULL;

//-----------------------------------------------------------------------------------------------------------------------------

SpotRepository::SpotRepository(QObject *parent) 
  : QObject(parent),
    m_RequestId(0),
    m_QMap_Spots(),
    m_QMap_Results()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

SpotRepository::~SpotRepository()
{
  foreach (int spotId, m_QMap_Spots.keys())
  {
    delete m_QMap_Spots.take(spotId);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::instanziate()
{
    if(s_SpotRepository != NULL)
    {
      qWarning() << "SpotRepository is already initialised.";
      return;
    }

    s_SpotRepository = new SpotRepository();

}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::destroy()
{
  if(s_SpotRepository == NULL)
  {
    qWarning() << "SpotRepository instance is already null.";
    return;
  }

  delete s_SpotRepository;
  s_SpotRepository = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------

int SpotRepository::getNewRequestId()
{
    return ++m_RequestId;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Spot *> SpotRepository::getSpots(int requestId)
{
  return m_QMap_Results.value(requestId);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::getBy_Distance(int requestId,
                                    double latitude,
                                    double longitude,
                                    double maxDistance_km)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_LATITUDE,        QString::number(latitude)));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_LONGITUDE,       QString::number(longitude)));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_MAX_DISTANCE_KM, QString::number(maxDistance_km)));

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::C_GET_SPOTS_BY_DISTANCE);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::getBy_UserId(int requestId,
                                  int userId)
{
    QList<QueryItem> qList_QueryItems;
    qList_QueryItems.append(QueryItem(WebApi::R_PARAM_USER_ID,  QString::number(userId)));

    // TODO check post return type
    WebApiCommand *webApiCommand = new WebApiCommand(this);
    webApiCommand->setAnswerType(WebApiCommand::JSON);
    webApiCommand->setCommand(WebApi::C_GET_SPOTS_BY_USER_ID);

    webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

    connect(webApiCommand,
            SIGNAL(signal_Finished(const WebApiError &)),
            SLOT(slot_Command_Finished(const WebApiError &)));
    webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::slot_Command_Finished(const WebApiError &error)
{
  QList<Spot *> newSpots;
  int requestId = sender()->property(PROPERTY_REQUEST_ID).toInt();

  if(error.type() != WebApiError::NONE)
  {
    emit signal_DataReady(requestId,
                          false);
    m_QMap_Results.insert(requestId,
                          QList<Spot *>());
    return;
  }

  WebApiCommand *webApiCommand = dynamic_cast<WebApiCommand *>(sender());
  QJsonArray jsonArray_Spots = webApiCommand->resultArray(WebApi::A_ARRAY_SPOTS);
  for(int i = 0; i < jsonArray_Spots.size(); i++)
  {
    QJsonObject jsonObject_Spot = jsonArray_Spots.at(i).toObject();

    int id = jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_ID).toInt();

    if(m_QMap_Spots.keys().contains(id) == false)
    {
      Spot *spot = new Spot(id,
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_NAME).toString(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION).toString(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE).toDouble(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE).toDouble(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE_KM).toDouble(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1).toString(),
                            jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2).toString(),
                            this);

      m_QMap_Spots.insert(id, spot);
    }
	else
    {
      m_QMap_Spots.value(id)->setName       (jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_NAME).toString());
      m_QMap_Spots.value(id)->setDescription(jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION).toString());
      m_QMap_Spots.value(id)->setLatitude   (jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE).toDouble());
      m_QMap_Spots.value(id)->setLongitude  (jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE).toDouble());
      m_QMap_Spots.value(id)->setPictureUrl1(jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1).toString());
      m_QMap_Spots.value(id)->setPictureUrl2(jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2).toString());

      // Set dinstance only if it was really computed
      double distance = jsonObject_Spot.value(WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE_KM).toDouble();
      if(distance > 0)
      {
        m_QMap_Spots.value(id)->setDistance(distance);
      }
    }

    newSpots.append(m_QMap_Spots.value(id));
  }

  webApiCommand->deleteLater();

  m_QMap_Results.insert(requestId,
                        newSpots);
  emit signal_DataReady(requestId,
                        true);
}

//-----------------------------------------------------------------------------------------------------------------------------

