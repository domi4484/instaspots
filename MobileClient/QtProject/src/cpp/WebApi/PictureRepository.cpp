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
#include "Spot.h"
#include "SpotRepository.h"
#include "UserRepository.h"
#include "WebApi.h"
#include "WebApiCommand.h"
#include "../HelperClasses/Logger.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const char* PictureRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository *PictureRepository::s_PictureRepository = NULL;

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository::PictureRepository(QObject *parent) :
  QObject(parent),
  m_RequestId(1),
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

Picture *PictureRepository::getBy_PictureId(int pictureId)
{
  if(pictureId < 0)
      return NULL;

  Picture *picture = m_QMap_Pictures.value(pictureId, NULL);
  if(picture == NULL)
  {
    picture = new Picture(this);
    picture->setId(pictureId);
    m_QMap_Pictures.insert(pictureId, picture);

    // #todo Get internet
  }

  return picture;
}

//-----------------------------------------------------------------------------------------------------------------------------

Picture *PictureRepository::getAdd_Picture(int pictureId)
{
  if(pictureId < 0)
      return NULL;

  Picture *picture = m_QMap_Pictures.value(pictureId, NULL);
  if(picture == NULL)
  {
    picture = new Picture(this);
    picture->setId(pictureId);
    m_QMap_Pictures.insert(pictureId, picture);

    // #todo Get internet
  }

  return picture;
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

    // #todo Get internet
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_SpotId(int requestId,
                                     int spotId)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_SPOT_ID,  QString::number(spotId)));

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::COMMAND::GET_PICTURES_BY_SPOT_ID);

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
   qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USER_ID,  QString::number(userId)));

   // TODO check post return type
   WebApiCommand *webApiCommand = new WebApiCommand(this);
   webApiCommand->setAnswerType(WebApiCommand::JSON);
   webApiCommand->setCommand(WebApi::COMMAND::GET_PICTURES_BY_USER_ID);

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
  webApiCommand->setCommand(WebApi::COMMAND::GET_PICTURES_BY_NEWEST);

  webApiCommand->setProperty(PROPERTY_REQUEST_ID, requestId);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::likePicture(int pictureId)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_PICTURE_ID,  QString::number(pictureId)));

  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::COMMAND::PICTURE_LIKE);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::unlikePicture(int pictureId)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_PICTURE_ID,  QString::number(pictureId)));

  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::COMMAND::PICTURE_UNLIKE);

  connect(webApiCommand,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_Command_Finished(const WebApiError &)));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::removePicture(int pictureId)
{
  Picture *picture = m_QMap_Pictures.value(pictureId,
                                           NULL);

  if(picture == NULL)
  {
    Logger::error(QString("Can't remove picture with id %1").arg(pictureId));
    return;
  }

  Logger::info(QString("Removing picture with id %1").arg(pictureId));

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_PICTURE_ID,  QString::number(pictureId)));

  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommand(WebApi::COMMAND::PICTURE_REMOVE);

  connect(webApiCommand,
          SIGNAL(signal_Finished()),
          webApiCommand,
          SLOT(deleteLater()));
  webApiCommand->postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::slot_Command_Finished(const WebApiError &error)
{
  QList<Picture *> newPictures;
  int requestId = sender()->property(PROPERTY_REQUEST_ID).toInt();

  if(   error.type() != WebApiError::NONE
     && requestId != 0)
  {
    emit signal_DataReady(requestId,
                          false);
    m_QMap_Results.insert(requestId,
                          QList<Picture *>());
    return;
  }

  WebApiCommand *webApiCommand = dynamic_cast<WebApiCommand *>(sender());
  QJsonArray jsonArray_Pictures = webApiCommand->resultArray(WebApi::PARAMETER::PICTURE_LIST);
  for(int i = 0; i < jsonArray_Pictures.size(); i++)
  {
    QJsonObject jsonObject_Picture = jsonArray_Pictures.at(i).toObject();

    int picture_id = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_PICTURE_ID).toInt();

    Picture *picture = m_QMap_Pictures.value(picture_id, NULL);
    if(picture == NULL)
    {
      picture = new Picture(this);
      picture->setId(picture_id);
      m_QMap_Pictures.insert(picture_id, picture);
    }

    // Picture properties
    picture->setUrl             (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_URL).toString());
    picture->setIdUser          (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_USER_ID).toInt());
    picture->setUsername        (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_USER_USERNAME).toString());
    picture->setCreated         (QDateTime::fromString(jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_CREATED).toString(),
                                                       Qt::ISODate));

    // Spot
    int     spot_id          = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_SPOT_ID).toInt();
    QString spot_name        = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_SPOT_NAME).toString();
    QString spot_description = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_SPOT_DESCRIPTION).toString();
    Spot *spot = SpotRepository::instance()->getAdd_Spot(spot_id,
                                                         spot_name,
                                                         spot_description);
    picture->setSpot(spot);

    // Likers
    picture->clearLikers();
    QJsonArray jsonArray_Likers = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_LIKERS).toArray();
    for(int i = 0; i < jsonArray_Likers.size(); i++)
    {
      QJsonObject jsonObject_Liker = jsonArray_Likers.at(i).toObject();

      int     liker_id       = jsonObject_Liker.value(WebApi::PARAMETER::USER_USER_ID).toInt();
      QString liker_username = jsonObject_Liker.value(WebApi::PARAMETER::USER_USERNAME).toString();

      User *user = UserRepository::instance()->getAdd_User(liker_id,
                                                           liker_username);

      picture->addLiker(user);
    }

    // Add to current request
    newPictures.append(picture);
  } // for

  webApiCommand->deleteLater();

  if(requestId != 0)
  {
    m_QMap_Results.insert(requestId,
                          newPictures);
    emit signal_DataReady(requestId,
                          true);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

