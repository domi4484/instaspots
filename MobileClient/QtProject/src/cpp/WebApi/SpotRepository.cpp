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
#include "../HelperClasses/LocationManager.h"
#include "Spot.h"
#include "WebApi.h"
#include "WebApiCommand.h"
#include "PictureRepository.h"
#include "User.h"
#include "UserRepository.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QJsonArray>
#include <QPointF>

//-----------------------------------------------------------------------------------------------------------------------------

const char* SpotRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

SpotRepository *SpotRepository::s_SpotRepository = NULL;

//-----------------------------------------------------------------------------------------------------------------------------

SpotRepository::SpotRepository(LocationManager *locationManger,
                               QObject *parent)
  : QObject(parent),
    m_LocationManager(locationManger),
    m_RequestId(1),
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

void SpotRepository::instanziate(LocationManager *locationManager)
{
    if(s_SpotRepository != NULL)
    {
      qWarning() << "SpotRepository is already initialised.";
      return;
    }

    s_SpotRepository = new SpotRepository(locationManager);
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

Spot *SpotRepository::getBy_SpotId(int spotId)
{
  if(spotId < 0)
      return NULL;

  Spot *spot = m_QMap_Spots.value(spotId, NULL);
  if(spot == NULL)
  {
    spot = new Spot(this);
    spot->setId(spotId);
    m_QMap_Spots.insert(spotId, spot);

    // Get from server
    QList<QueryItem> qList_QueryItems;
    qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_SPOT_ID,    QString::number(spotId)));

    // TODO check post return type
    WebApiCommand *webApiCommand = new WebApiCommand(this);
    webApiCommand->setAnswerType(WebApiCommand::JSON);
    webApiCommand->setCommandName(WebApi::COMMAND::GET_SPOT_BY_ID);

    webApiCommand->setProperty(PROPERTY_REQUEST_ID, getNewRequestId());

    connect(webApiCommand,
            SIGNAL(signal_Finished(const WebApiError &)),
            SLOT(slot_Command_Finished(const WebApiError &)));
    webApiCommand->postRequest(qList_QueryItems);
  }

  return spot;
}

//-----------------------------------------------------------------------------------------------------------------------------

Spot *SpotRepository::getAdd_Spot(int spotId,
                                  const QString &spotName,
                                  const QString &spotDescription)
{
  if(spotId < 0)
    return NULL;

  Spot *spot = m_QMap_Spots.value(spotId, NULL);
  if(spot == NULL)
  {
    spot = new Spot(this);
    spot->setId(spotId);
    m_QMap_Spots.insert(spotId, spot);

    // Get from server
    QList<QueryItem> qList_QueryItems;
    qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_SPOT_ID,    QString::number(spotId)));

    // TODO check post return type
    WebApiCommand *webApiCommand = new WebApiCommand(this);
    webApiCommand->setAnswerType(WebApiCommand::JSON);
    webApiCommand->setCommandName(WebApi::COMMAND::GET_SPOT_BY_ID);

    webApiCommand->setProperty(PROPERTY_REQUEST_ID, getNewRequestId());

    connect(webApiCommand,
            SIGNAL(signal_Finished(const WebApiError &)),
            SLOT(slot_Command_Finished(const WebApiError &)));
    webApiCommand->postRequest(qList_QueryItems);
  }
  spot->setName(spotName);
  spot->setDescription(spotDescription);

  return spot;
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotRepository::getBy_Distance(int requestId,
                                    double latitude,
                                    double longitude,
                                    double maxDistance_m)
{
  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LATITUDE,    QString::number(latitude)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LONGITUDE,   QString::number(longitude)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_DISTANCE_KM, QString::number(maxDistance_m/1000.0)));

  // TODO check post return type
  WebApiCommand *webApiCommand = new WebApiCommand(this);
  webApiCommand->setAnswerType(WebApiCommand::JSON);
  webApiCommand->setCommandName(WebApi::COMMAND::GET_SPOTS_BY_DISTANCE);

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
    qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USER_ID,  QString::number(userId)));

    // TODO check post return type
    WebApiCommand *webApiCommand = new WebApiCommand(this);
    webApiCommand->setAnswerType(WebApiCommand::JSON);
    webApiCommand->setCommandName(WebApi::COMMAND::GET_SPOTS_BY_USER_ID);

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

  if(   error.type() != WebApiError::NONE
     && requestId != 0)
  {
    emit signal_DataReady(requestId,
                          false);
    m_QMap_Results.insert(requestId,
                          QList<Spot *>());
    sender()->deleteLater();
    return;
  }

  WebApiCommand *webApiCommand = dynamic_cast<WebApiCommand *>(sender());
  QJsonArray jsonArray_Spots = webApiCommand->resultArray(WebApi::PARAMETER::SPOT_LIST);
  for(int i = 0; i < jsonArray_Spots.size(); i++)
  {
    QJsonObject jsonObject_Spot = jsonArray_Spots.at(i).toObject();

    int spot_id = jsonObject_Spot.value(WebApi::PARAMETER::SPOT_SPOT_ID).toInt();

    Spot *spot = m_QMap_Spots.value(spot_id, NULL);
    if(spot == NULL)
    {
      spot = new Spot(this);
      spot->setId(spot_id);
      m_QMap_Spots.insert(spot_id, spot);
    }

    spot->setName       (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_NAME                 ).toString());
    spot->setDescription(jsonObject_Spot.value(WebApi::PARAMETER::SPOT_DESCRIPTION          ).toString());
    spot->setSecretSpot (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_SECRET_SPOT          ).toBool());
    spot->setLatitude   (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_LATITUDE             ).toDouble());
    spot->setLongitude  (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_LONGITUDE            ).toDouble());
    spot->setPictureUrl1(jsonObject_Spot.value(WebApi::PARAMETER::SPOT_PICTURE_URL_1        ).toString());
    spot->setPictureUrl2(jsonObject_Spot.value(WebApi::PARAMETER::SPOT_PICTURE_URL_2        ).toString());
    spot->setPictureId1 (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_1 ).toInt());
    spot->setPictureId2 (jsonObject_Spot.value(WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_2 ).toInt());

    // Set dinstance if available, compute it or leave it empty
    if(m_LocationManager->isValid())
    {
      qreal computedDistance = m_LocationManager->computeDistance(QPointF(m_LocationManager->latitude(),
                                                                          m_LocationManager->longitude()),
                                                                  QPointF(spot->latitude(),
                                                                          spot->longitude()));
      spot->setDistance(computedDistance);
    }

    // Picture list
    spot->clearPictures();
    QJsonArray jsonArray_Pictures = jsonObject_Spot.value(WebApi::PARAMETER::PICTURE_LIST).toArray();
    for(int i = 0; i < jsonArray_Pictures.size(); i++)
    {
      QJsonObject jsonObject_Picture = jsonArray_Pictures.at(i).toObject();

      int picture_id = jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_PICTURE_ID).toInt();

      Picture *picture = PictureRepository::instance()->getAdd_Picture(picture_id);

      // Picture properties
      picture->setUrl             (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_URL).toString());
      picture->setIdUser          (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_USER_ID).toInt());
      picture->setUsername        (jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_USER_USERNAME).toString());
      picture->setCreated         (QDateTime::fromString(jsonObject_Picture.value(WebApi::PARAMETER::PICTURE_CREATED).toString(),
                                                         Qt::ISODate));

      // Spot
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

      spot->addPicture(picture);
    } // for

    // Add to current request
    newSpots.append(m_QMap_Spots.value(spot_id));
  }

  webApiCommand->deleteLater();

  if(requestId != 0)
  {
    m_QMap_Results.insert(requestId,
                          newSpots);
    emit signal_DataReady(requestId,
                          true);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

