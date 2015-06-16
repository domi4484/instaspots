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
#include "PictureRepository.h"

// Project includes ------------------------
#include "Picture.h"
#include "WebApi.h"
#include "WebApiCommand.h"

//-----------------------------------------------------------------------------------------------------------------------------

const char* PictureRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository::PictureRepository(QObject *parent)
  : QObject(parent),
    m_RequestId(0),
    m_QMap_Pictures(),
    m_QMap_Results()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository::~PictureRepository()
{
  foreach (int pictureId, m_QMap_Pictures.keys())
  {
    delete m_QMap_Pictures.take(pictureId);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Picture *> PictureRepository::getPictures(int requestId)
{
  return m_QMap_Results.value(requestId,
                              QList<Picture *>());
}

//-----------------------------------------------------------------------------------------------------------------------------

int PictureRepository::getBy_SpotId(int spotId)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_SPOT_ID,  QString::number(spotId)));

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::C_GET_PICTURES);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, ++m_RequestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);

  return m_RequestId;
}

//-----------------------------------------------------------------------------------------------------------------------------

int PictureRepository::getNews()
{
  QList<QueryItem> qList_QueryItems;

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::C_GET_NEWS);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, ++m_RequestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);

  return m_RequestId;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::slot_Command_Finished(const WebApiError &error)
{
  QList<Picture *> newPictures;
  int requestId = sender()->property(PROPERTY_REQUEST_ID).toInt();

  if(error.type() != WebApiError::NONE)
  {
    emit signal_DataReady(requestId,
                          false);
    m_QMap_Results.insert(requestId,
                          QList<Picture *>());
    return;
  }

  WebApiCommand *webApiCommand = dynamic_cast<WebApiCommand *>(sender());
  QScriptValue qScriptValue_Ids = webApiCommand->resultProperty(WebApi::A_ARRAY_PICTURES);
  int length = qScriptValue_Ids.property("length").toInteger();

  for(int i = 0; i < length; i++)
  {
    int id = qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID).toInteger();

    if(m_QMap_Pictures.keys().contains(id) == false)
    {
      Picture *picture = new Picture(id,
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_USER).toInteger(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_SPOT).toInteger(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_URL).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_USERNAME).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_NAME).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_DESCRIPTION).toString(),
                                     QDateTime::fromString(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_CREATED).toString(),
                                                           Qt::ISODate));

      m_QMap_Pictures.insert(picture->id(), picture);
    }
    newPictures.append(m_QMap_Pictures.value(id));
  }

  webApiCommand->deleteLater();

  m_QMap_Results.insert(requestId,
                        newPictures);
  emit signal_DataReady(requestId,
                        true);
}

//-----------------------------------------------------------------------------------------------------------------------------

