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
      qWarning() << "Logger is already initialised.";
      return;
    }

    s_SpotRepository = new SpotRepository();

}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::destroy()
{
  if(s_SpotRepository == NULL)
  {
    qWarning() << "Logger instance is already null.";
    return;
  }

  delete s_SpotRepository;
  s_SpotRepository = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Spot *> SpotRepository::getSpots(int requestId)
{
  return m_QMap_Results.value(requestId,
                              QList<Spot *>());
}

//-----------------------------------------------------------------------------------------------------------------------------

int SpotRepository::getByDistance(double latitude,
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
  webApiCommand->setCommand(WebApi::C_GET_NEARBY_SPOTS);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, ++m_RequestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);

  return m_RequestId;
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
  QScriptValue qScriptValue_Ids = webApiCommand->resultProperty(WebApi::A_ARRAY_SPOTS);
  int length = qScriptValue_Ids.property("length").toInteger();

  for(int i = 0; i < length; i++)
  {
    int id = qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_ID).toInteger();

    if(m_QMap_Spots.keys().contains(id) == false)
    {
      Spot *spot = new Spot(id,
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_NAME).toString(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION).toString(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE).toNumber(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE).toNumber(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE_KM).toNumber(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1).toString(),
                            qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2).toString());

      m_QMap_Spots.insert(id, spot);
    }
	else
    {
      m_QMap_Spots.value(id)->setName       (qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_NAME).toString());
      m_QMap_Spots.value(id)->setDescription(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION).toString());
      m_QMap_Spots.value(id)->setLatitude   (qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE).toNumber());
      m_QMap_Spots.value(id)->setLongitude  (qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE).toNumber());
      m_QMap_Spots.value(id)->setDistance   (qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE_KM).toNumber());
      m_QMap_Spots.value(id)->setPictureUrl1(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1).toString());
      m_QMap_Spots.value(id)->setPictureUrl2(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2).toString());
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

