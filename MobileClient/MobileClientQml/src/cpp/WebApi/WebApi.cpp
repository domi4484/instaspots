/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.05.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "WebApi.h"

// Project includes ------------------------
#include "WebApiCommand.h"
#include "WebApiError.h"

// Library includes ------------------------
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QHttpMultiPart>

//-----------------------------------------------------------------------------------------------------------------------------

const QString WebApi::CONST::PATH  ("api");

const QString WebApi::CONST::GENERAL_PARAMS::COMMAND    ("command");
const QString WebApi::CONST::GENERAL_PARAMS::ERROR      ("error");

const char *WebApi::PROPERTY_COMMAND_ID = "command_id";

// Commands
const QString WebApi::COMMAND::LOGIN       = "login";
const QString WebApi::COMMAND::LOGOUT      = "logout";
const QString WebApi::COMMAND::CANREGISTER = "canregister";
const QString WebApi::COMMAND::REGISTER    = "register";

const QString WebApi::COMMAND::REPORT_PROBLEM             = "reportProblem";
const QString WebApi::COMMAND::GET_CURRENT_CLIENT_VERSION = "getCurrentClientVersion";

const QString WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT = "uploadPictureToSpot";
const QString WebApi::COMMAND::UPLOAD_NEW_SPOT        = "uploadNewSpot";

const QString WebApi::COMMAND::GET_PICTURES_BY_NEWEST  = "pictures/byNewest";
const QString WebApi::COMMAND::GET_PICTURES_BY_SPOT_ID = "getPicturesBySpotId";
const QString WebApi::COMMAND::GET_PICTURES_BY_USER_ID = "getPicturesByUserId";

const QString WebApi::COMMAND::GET_SPOTS = "spots";
const QString WebApi::COMMAND::GET_SPOTS_BY_DISTANCE   = "getSpotsByDistance";
const QString WebApi::COMMAND::GET_SPOTS_BY_USER_ID    = "getSpotsByUserId";

const QString WebApi::COMMAND::SPOT_SIGNAL_DISAPPEARED = "spotSignalDisappeared";

const QString WebApi::COMMAND::PICTURE_LIKE   = "pictureLike";
const QString WebApi::COMMAND::PICTURE_UNLIKE = "pictureUnlike";
const QString WebApi::COMMAND::PICTURE_REMOVE = "pictureRemove";

// Parameters
const QString WebApi::PARAMETER::USER_LIST          = "user_list";
const QString WebApi::PARAMETER::USER_USER_ID       = "user_userId";
const QString WebApi::PARAMETER::USER_USERNAME      = "user_username";
const QString WebApi::PARAMETER::USER_PASSWORD      = "user_password";
const QString WebApi::PARAMETER::USER_EMAIL         = "user_email";
const QString WebApi::PARAMETER::USER_AUTHENTICATED = "user_authenticated";
const QString WebApi::PARAMETER::USER_REGISTERED    = "user_registered";

const QString WebApi::PARAMETER::REPORT_TITLE   = "reportTitle";
const QString WebApi::PARAMETER::REPORT_CONTENT = "reportContent";

const QString WebApi::PARAMETER::APPLICATION_VERSION = "application_version";
const QString WebApi::PARAMETER::PARAM_SUCCESSFUL    = "successful";

const QString WebApi::PARAMETER::SPOT_LIST                 = "spots";
const QString WebApi::PARAMETER::SPOT_SPOT_ID              = "id";
const QString WebApi::PARAMETER::SPOT_NAME                 = "name";
const QString WebApi::PARAMETER::SPOT_DESCRIPTION          = "description";
const QString WebApi::PARAMETER::SPOT_SECRET_SPOT          = "secretspot";
const QString WebApi::PARAMETER::SPOT_LATITUDE             = "latitude";
const QString WebApi::PARAMETER::SPOT_LONGITUDE            = "longitude";
const QString WebApi::PARAMETER::SPOT_TAGS                 = "tags";
const QString WebApi::PARAMETER::SPOT_SCORE                = "score";
const QString WebApi::PARAMETER::SPOT_DISTANCE_KM          = "DistanceKm";
const QString WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_1 = "picture1_id";
const QString WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_2 = "picture2_is";
const QString WebApi::PARAMETER::SPOT_PICTURE_URL_1        = "spot_picture_url_1";
const QString WebApi::PARAMETER::SPOT_PICTURE_URL_2        = "spot_picture_url_2";

const QString WebApi::PARAMETER::PICTURE_LIST             = "pictures";
const QString WebApi::PARAMETER::PICTURE_PICTURE_ID       = "id";
const QString WebApi::PARAMETER::PICTURE_LATITUDE         = "latitude";
const QString WebApi::PARAMETER::PICTURE_LONGITUDE        = "longitude";
const QString WebApi::PARAMETER::PICTURE_URL              = "url";
const QString WebApi::PARAMETER::PICTURE_CREATED          = "created";
const QString WebApi::PARAMETER::PICTURE_LIKERS           = "likers";
const QString WebApi::PARAMETER::PICTURE_SPOT_ID          = "spot";
const QString WebApi::PARAMETER::PICTURE_SPOT_NAME        = "spot_name";
const QString WebApi::PARAMETER::PICTURE_SPOT_DESCRIPTION = "spot_description";
const QString WebApi::PARAMETER::PICTURE_SPOT_SCORE       = "spot_score";
const QString WebApi::PARAMETER::PICTURE_USER_ID          = "user";
const QString WebApi::PARAMETER::PICTURE_USER_USERNAME    = "user_name";

const QString WebApi::PARAMETER::A_PARAM_SUCCESSFUL  ("successful");

//-----------------------------------------------------------------------------------------------------------------------------

WebApi *WebApi::sInstance(NULL);

//-----------------------------------------------------------------------------------------------------------------------------

WebApi::WebApi() :
  QObject(),
  mServerAddress(),
  mUltraNetworkAccessManager(),
  mCommandsIdCounter(0),
  mRunningCommands()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApi::~WebApi()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApi *WebApi::instance()
{
  if(sInstance == NULL)
    sInstance = new WebApi();

  return sInstance;
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApi::destroy()
{
  if(sInstance == NULL)
    return;

  delete sInstance;
  sInstance = NULL;
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::setUrl(const QString url)
{
  mServerAddress = url;
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::slot_QNetworkReply_uploadProgress(qint64 received,
                                               qint64 total)
{
  // Retrieve command
  QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
  QNetworkReply::NetworkError replyNetworkError = reply->error();
  unsigned int commandId = reply->property(PROPERTY_COMMAND_ID).toUInt();
  WebApiCommand *command = mRunningCommands.value(commandId, NULL);

  // Command id not found
  if(command == NULL)
  {
    Logger::error(tr("Command with id '%1' not found in command list.").arg(commandId));
    return;
  }

  // Network error
  if ( replyNetworkError != QNetworkReply::NoError )
  {
    Logger::error(QString("Network error %1").arg(replyNetworkError));
    command->setResult(WebApiError(WebApiError::NETWORK),
                       QJsonDocument());
    return;
  }

  if(   received != 0
     && total    != 0)
  {
    Logger::verbose(QString("%1/%2").arg(received)
                  .arg(total));
    command->setProgress(received,
                         total);
    return;
  }

  // Delete reply
  reply->deleteLater();

  // Received data
  QByteArray data = reply->readAll();
  QString dataText = data;
  Logger::verbose(QString("Received: %1").arg(dataText));

  // Parse received JSON
  QJsonParseError jsonParseError;
  QJsonDocument jsonDocument = QJsonDocument::fromJson(data,
                                                       &jsonParseError);

  if(jsonParseError.error != QJsonParseError::NoError)
  {
      Logger::error(tr("Parse Json error: %1").arg(jsonParseError.errorString()));
      command->setResult(WebApiError(WebApiError::SERVER),
                         jsonDocument);
      return;
  }

  if(jsonDocument.object().isEmpty())
  {
      Logger::error(tr("Server error: received an empty answer").arg(jsonParseError.errorString()));
      command->setResult(WebApiError(WebApiError::SERVER),
                         jsonDocument);
      return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     jsonDocument);
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::slot_QNetworkReply_downloadProgress(qint64 received,
                                                 qint64 total)
{
  // Retrieve command
  QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
  QNetworkReply::NetworkError replyNetworkError = reply->error();
  unsigned int commandId = reply->property(PROPERTY_COMMAND_ID).toUInt();
  WebApiCommand *command = mRunningCommands.value(commandId, NULL);

  // Command id not found
  if(command == NULL)
  {
    Logger::error(tr("Command with id '%1' not found in command list.").arg(commandId));
    return;
  }

  // Network error
  if ( replyNetworkError != QNetworkReply::NoError )
  {
    Logger::error(QString("Network error %1").arg(replyNetworkError));
    command->setResult(WebApiError(WebApiError::NETWORK),
                       QJsonDocument());
    return;
  }

  if(received != total)
  {
    Logger::verbose(QString("%1/%2").arg(received)
                                  .arg(total));
    command->setProgress(received,
                         total);
    return;
  }

  // Delete reply
  reply->deleteLater();

  // Received data
  command->setRawResult(WebApiError(WebApiError::NONE),
                        reply->readAll());
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::slot_QNetworkReply_finished()
{
  // Retrieve command and data
  QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
  QNetworkReply::NetworkError replyNetworkError       = reply->error();
  QString                     replyNetworkErrorString = reply->errorString();
  unsigned int commandId = reply->property(PROPERTY_COMMAND_ID).toUInt();
  WebApiCommand *command = mRunningCommands.value(commandId, NULL);
  reply->deleteLater();

  // Command id not found
  if(command == NULL)
  {
    Logger::error(tr("Command with id '%1' not found in command list.").arg(commandId));
    return;
  }

  // Network error
  if ( replyNetworkError != QNetworkReply::NoError )
  {   
    Logger::error(QString("Network error %1 (%2). Request: '%3'").arg(replyNetworkError)
                                                                 .arg(replyNetworkErrorString)
                                                                 .arg(command->requestString()));
    command->setResult(WebApiError(WebApiError::NETWORK),
                       QJsonDocument());
    return;
  }

  // Received data
  QByteArray data = reply->readAll();
  QString dataText = data;
  Logger::verbose(QString("Received: %1").arg(dataText));

  // Parse received JSON
  QJsonParseError jsonParseError;
  QJsonDocument jsonDocument = QJsonDocument::fromJson(data,
                                                       &jsonParseError);

  if(jsonParseError.error != QJsonParseError::NoError)
  {
      Logger::error(tr("Parse Json error: %1").arg(jsonParseError.errorString()));
      command->setResult(WebApiError(WebApiError::SERVER),
                         jsonDocument);
      return;
  }

  // Command error
  QString errorText = jsonDocument.object().value(CONST::GENERAL_PARAMS::ERROR).toString();
  if(errorText.isEmpty() == false)
  {
    Logger::error("Error: " + errorText);
    command->setResult(WebApiError(WebApiError::COMMAND,
                                   errorText),
                       jsonDocument);
    return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     jsonDocument);
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::sendRequest(WebApiCommand *abstractCommand,
                         QList<QueryItem> &qList_QueryItems)
{
  int commandId = mCommandsIdCounter++;
  mRunningCommands.insert(commandId, abstractCommand);

  qList_QueryItems.append(QueryItem(PARAMETER::APPLICATION_VERSION,
                                    QApplication::applicationVersion()));

  // Network request
  QNetworkRequest request(QString("%1/%2/%3/").arg(mServerAddress)
                                              .arg(CONST::PATH)
                                              .arg(abstractCommand->commandName()));

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem(CONST::GENERAL_PARAMS::COMMAND, abstractCommand->commandName());
  foreach(QueryItem item, qList_QueryItems)
  {
    params.addQueryItem(item.first(), item.second());
  }

  QNetworkReply *reply = nullptr;
  switch (abstractCommand->requestType())
  {
    case WebApiCommand::RequestTypePost:
      reply = mUltraNetworkAccessManager.post(request, params.query(QUrl::FullyEncoded).toUtf8());
    break;
    case WebApiCommand::RequestTypeGet:
      reply = mUltraNetworkAccessManager.get(request);
    break;
  }

  reply->setProperty(PROPERTY_COMMAND_ID, commandId);
  connect(reply,
          SIGNAL(finished()),
          SLOT  (slot_QNetworkReply_finished()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApi::sendMultipartRequest(WebApiCommand *abstractCommand,
                              QList<QueryItem> &qList_QueryItems,
                              QIODevice *device)
{
  int commandId = mCommandsIdCounter++;
  mRunningCommands.insert(commandId, abstractCommand);

  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::APPLICATION_VERSION,
                                    QApplication::applicationVersion()));

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  // Command
  {
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("application/x-www-form-urlencoded; name=\"%1\";").arg(CONST::GENERAL_PARAMS::COMMAND)));
    textPart.setBody(abstractCommand->commandName().toUtf8());

    multiPart->append(textPart);
  }

  // Text part
  foreach (QueryItem queryItem, qList_QueryItems)
  {
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("application/x-www-form-urlencoded; name=\"%1\";").arg(queryItem.first())));
    textPart.setBody(queryItem.second().toUtf8());

    multiPart->append(textPart);
  }

  // Image part
  QHttpPart imagePart;
  imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("image/jpeg"));
  imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                      QVariant(  "application/x-www-form-urlencoded; name=\"image\"; filename=\"ilmiobelfile.jpg\""));
  device->open(QIODevice::ReadOnly);
  imagePart.setBodyDevice(device);
  device->setParent(multiPart); // we cannot delete the device now, so delete it with the multiPart

  multiPart->append(imagePart);

  // Network request
  QNetworkRequest request(QString("%1/%2/%3").arg(mServerAddress)
                          .arg(CONST::PATH)
                          .arg(abstractCommand->commandName()));

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  QNetworkReply *reply = mUltraNetworkAccessManager.post(request, multiPart);
  reply->setProperty(PROPERTY_COMMAND_ID, commandId);

  multiPart->setParent(reply); // delete the multiPart with the reply

  connect(reply,
          SIGNAL(uploadProgress(qint64, qint64)),
          SLOT  (slot_QNetworkReply_uploadProgress(qint64, qint64)));
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApi::downloadRequest(WebApiCommand *abstractCommand,
                             const QString &url)
{
  int commandId = mCommandsIdCounter++;
  mRunningCommands.insert(commandId, abstractCommand);

  // Network request
  QNetworkRequest request(url);

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  QNetworkReply *reply = mUltraNetworkAccessManager.get(request);
  reply->setProperty(PROPERTY_COMMAND_ID, commandId);

  connect(reply,
          SIGNAL(downloadProgress(qint64, qint64)),
          SLOT  (slot_QNetworkReply_downloadProgress(qint64, qint64)));
}

//-----------------------------------------------------------------------------------------------------------------------------







