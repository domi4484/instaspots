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

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const char* PictureRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository *PictureRepository::s_PictureRepository = NULL;

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

void PictureRepository::instanziate()
{
    if(s_PictureRepository != NULL)
    {
      qWarning() << "PictureRepository is already initialised.";
      return;
    }

    s_PictureRepository = new PictureRepository();

}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::destroy()
{
  if(s_PictureRepository == NULL)
  {
    qWarning() << "PictureRepository instance is already null.";
    return;
  }

  delete s_PictureRepository;
  s_PictureRepository = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------

int PictureRepository::getNewRequestId()
{
    return ++m_RequestId;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Picture *> PictureRepository::getPictures(int requestId)
{
  return m_QMap_Results.value(requestId);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_PictureId(int requestId,
                                        int pictureId)
{
    // Get local
    if(m_QMap_Pictures.contains(pictureId))
    {
        QList<Picture *> newPictures;
        newPictures.append(m_QMap_Pictures.value(pictureId));
        m_QMap_Results.insert(requestId,
                              newPictures);
        emit signal_DataReady(requestId,
                              true);
        return;
    }

    // Get internet
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_SpotId(int requestId,
                                     int spotId)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_SPOT_ID,  QString::number(spotId)));

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::C_GET_PICTURES_BY_SPOT_ID);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_UserId(int requestId,
                                     int userId)
{
   QList<QueryItem> qList_QueryItems;
   qList_QueryItems.append(QueryItem(WebApi::R_PARAM_USER_ID,  QString::number(userId)));

   // TODO check post return type
   WebApiCommand *webApiCommand = new WebApiCommand(this);
   webApiCommand->setAnswerType(WebApiCommand::JSON);
   webApiCommand->setCommand(WebApi::C_GET_PICTURES_BY_USER_ID);

   webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

   connect(webApiCommand,
           SIGNAL(signal_Finished(const WebApiError &)),
           SLOT(slot_Command_Finished(const WebApiError &)));
   webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_Newest(int requestId)
{
  QList<QueryItem> qList_QueryItems;

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::C_GET_PICTURES_BY_NEWEST);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
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
  QJsonArray jsonArray_Pictures = webApiCommand->resultArray(WebApi::A_ARRAY_PICTURES);
  for(int i = 0; i < jsonArray_Pictures.size(); i++)
  {
    QJsonObject jsonObject_Picture = jsonArray_Pictures.at(i).toObject();

    int picture_id = jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_ID).toInt();

    Picture *picture = m_QMap_Pictures.value(picture_id, NULL);
    if(picture == NULL)
    {
      picture = new Picture(this);
      picture->setIdSpot(picture_id);
      m_QMap_Pictures.insert(picture_id, picture);
    }

    picture->setIdUser          (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_USER).toInt());
    picture->setIdSpot          (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_SPOT).toInt());
    picture->setUrl             (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_URL).toString());
    picture->setUsername        (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_USERNAME).toString());
    picture->setSpotName        (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_NAME).toString());
    picture->setSpotDescription (jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_DESCRIPTION).toString());
    picture->setCreated         (QDateTime::fromString(jsonObject_Picture.value(WebApi::A_ARRAY_PICTURES_ELEMENT_CREATED).toString(),
                                                       Qt::ISODate));

    // Add to current request
    newPictures.append(picture);
  }

  webApiCommand->deleteLater();

  m_QMap_Results.insert(requestId,
                        newPictures);
  emit signal_DataReady(requestId,
                        true);
}

//-----------------------------------------------------------------------------------------------------------------------------

