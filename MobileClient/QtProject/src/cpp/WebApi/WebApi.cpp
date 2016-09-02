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
#include "../HelperClasses/Logger.h"
#include "WebApiCommand.h"
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QFile>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QHttpMultiPart>

//-----------------------------------------------------------------------------------------------------------------------------

// Location of the web service
//const QString WebApi::URL_DEVELOPMENT ("http://localhost/Symfony/web/app_dev.php/webservice");
const QString WebApi::URL_DEVELOPMENT ("http://127.0.0.1:8000/webservice/webservice/");
const QString WebApi::URL_PRODUCTION  ("https://lowerspot.com/web/webservice");


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

const QString WebApi::COMMAND::GET_PICTURES_BY_NEWEST  = "getPicturesByNewest";
const QString WebApi::COMMAND::GET_PICTURES_BY_SPOT_ID = "getPicturesBySpotId";
const QString WebApi::COMMAND::GET_PICTURES_BY_USER_ID = "getPicturesByUserId";
const QString WebApi::COMMAND::GET_SPOTS_BY_DISTANCE   = "getSpotsByDistance";
const QString WebApi::COMMAND::GET_SPOTS_BY_USER_ID    = "getSpotsByUserId";

const QString WebApi::COMMAND::SPOT_SIGNAL_DISAPPEARED = "spotSignalDisappeared";

const QString WebApi::COMMAND::PICTURE_LIKE   = "pictureLike";
const QString WebApi::COMMAND::PICTURE_UNLIKE = "pictureUnlike";
const QString WebApi::COMMAND::PICTURE_REMOVE = "pictureRemove";

// Parameters
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

const QString WebApi::PARAMETER::SPOT_LIST                 = "spot_list";
const QString WebApi::PARAMETER::SPOT_SPOT_ID              = "spot_spotId";
const QString WebApi::PARAMETER::SPOT_NAME                 = "spot_name";
const QString WebApi::PARAMETER::SPOT_DESCRIPTION          = "spot_description";
const QString WebApi::PARAMETER::SPOT_SECRET_SPOT          = "spot_secretSpot";
const QString WebApi::PARAMETER::SPOT_LATITUDE             = "spot_latitude";
const QString WebApi::PARAMETER::SPOT_LONGITUDE            = "spot_longitude";
const QString WebApi::PARAMETER::SPOT_TAGS                 = "spot_Tags";
const QString WebApi::PARAMETER::SPOT_SCORE                = "spot_Score";
const QString WebApi::PARAMETER::SPOT_DISTANCE_KM          = "spot_DistanceKm";
const QString WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_1 = "spot_picture_pictureId_1";
const QString WebApi::PARAMETER::SPOT_PICTURE_PICTURE_ID_2 = "spot_picture_pictureId_2";
const QString WebApi::PARAMETER::SPOT_PICTURE_URL_1        = "spot_picture_url_1";
const QString WebApi::PARAMETER::SPOT_PICTURE_URL_2        = "spot_picture_url_2";

const QString WebApi::PARAMETER::PICTURE_LIST             = "picture_list";
const QString WebApi::PARAMETER::PICTURE_PICTURE_ID       = "picture_pictureId";
const QString WebApi::PARAMETER::PICTURE_LATITUDE         = "picture_latitude";
const QString WebApi::PARAMETER::PICTURE_LONGITUDE        = "picture_longitude";
const QString WebApi::PARAMETER::PICTURE_URL              = "picture_url";
const QString WebApi::PARAMETER::PICTURE_CREATED          = "picture_created";
const QString WebApi::PARAMETER::PICTURE_LIKERS           = "picture_likers";
const QString WebApi::PARAMETER::PICTURE_SPOT_ID          = "picture_spot_spotId";
const QString WebApi::PARAMETER::PICTURE_SPOT_NAME        = "picture_spot_name";
const QString WebApi::PARAMETER::PICTURE_SPOT_DESCRIPTION = "picture_spot_description";
const QString WebApi::PARAMETER::PICTURE_SPOT_SCORE       = "picture_spot_score";
const QString WebApi::PARAMETER::PICTURE_USER_ID          = "picture_user_userId";
const QString WebApi::PARAMETER::PICTURE_USER_USERNAME    = "picture_user_username";

const QString WebApi::PARAMETER::A_PARAM_SUCCESSFUL  ("successful");

//-----------------------------------------------------------------------------------------------------------------------------

WebApi *WebApi::s_Instance(NULL);

//-----------------------------------------------------------------------------------------------------------------------------

WebApi::WebApi() :
  QObject(),
  m_Url(URL_PRODUCTION),
  m_UltraNetworkAccessManager(),
  m_CommandsIdCounter(0),
  m_RunningCommands()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApi::~WebApi()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApi *WebApi::instance()
{
  if(s_Instance == NULL)
    s_Instance = new WebApi();

  return s_Instance;
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApi::destroy()
{
  if(s_Instance == NULL)
    return;

  delete s_Instance;
  s_Instance = NULL;
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::setUrl(const QString url)
{
  m_Url.setUrl(url);
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::slot_QNetworkReply_uploadProgress(qint64 received,
                                               qint64 total)
{
  // Retrieve command
  QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
  QNetworkReply::NetworkError replyNetworkError = reply->error();
  unsigned int commandId = reply->property(PROPERTY_COMMAND_ID).toUInt();
  WebApiCommand *command = m_RunningCommands.value(commandId, NULL);

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
                       QJsonObject());
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
                         jsonDocument.object());
      return;
  }

  if(jsonDocument.object().isEmpty())
  {
      Logger::error(tr("Server error: received an empty answer").arg(jsonParseError.errorString()));
      command->setResult(WebApiError(WebApiError::SERVER),
                         jsonDocument.object());
      return;
  }

  // Command error
  QString errorText = jsonDocument.object().value(CONST::GENERAL_PARAMS::ERROR).toString();
  if(errorText.isEmpty() == false)
  {
    Logger::error("Error: " + errorText);
    command->setResult(WebApiError(WebApiError::COMMAND,
                                   errorText),
                       jsonDocument.object());
    return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     jsonDocument.object());
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::slot_QNetworkReply_downloadProgress(qint64 received,
                                                 qint64 total)
{
  // Retrieve command
  QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
  QNetworkReply::NetworkError replyNetworkError = reply->error();
  unsigned int commandId = reply->property(PROPERTY_COMMAND_ID).toUInt();
  WebApiCommand *command = m_RunningCommands.value(commandId, NULL);

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
                       QJsonObject());
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
  WebApiCommand *command = m_RunningCommands.value(commandId, NULL);
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
                       QJsonObject());
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
                         jsonDocument.object());
      return;
  }

  if(jsonDocument.object().isEmpty())
  {
      Logger::error(tr("Server error: received an empty answer").arg(jsonParseError.errorString()));
      command->setResult(WebApiError(WebApiError::SERVER),
                         jsonDocument.object());
      return;
  }

  // Command error
  QString errorText = jsonDocument.object().value(CONST::GENERAL_PARAMS::ERROR).toString();
  if(errorText.isEmpty() == false)
  {
    Logger::error("Error: " + errorText);
    command->setResult(WebApiError(WebApiError::COMMAND,
                                   errorText),
                       jsonDocument.object());
    return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     jsonDocument.object());
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::postRequest(WebApiCommand *abstractCommand,
                         const QList<QueryItem> &qList_QueryItems)
{
  int commandId = m_CommandsIdCounter++;
  m_RunningCommands.insert(commandId, abstractCommand);

  // Network request
  QNetworkRequest request(m_Url);

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem(CONST::GENERAL_PARAMS::COMMAND,    abstractCommand->command());
  foreach(QueryItem item, qList_QueryItems)
  {
    params.addQueryItem(item.first(), item.second());
  }

  QNetworkReply *reply = m_UltraNetworkAccessManager.post(request, params.query(QUrl::FullyEncoded).toUtf8());
  reply->setProperty(PROPERTY_COMMAND_ID, commandId);
  connect(reply,
          SIGNAL(finished()),
          SLOT  (slot_QNetworkReply_finished()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApi::multipartRequest(WebApiCommand *abstractCommand,
                              const QList<QueryItem> &qList_QueryItems,
                              QIODevice *device)
{
  int commandId = m_CommandsIdCounter++;
  m_RunningCommands.insert(commandId, abstractCommand);

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  // Command
  {
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant(QString("application/x-www-form-urlencoded; name=\"%1\";").arg(CONST::GENERAL_PARAMS::COMMAND)));
    textPart.setBody(abstractCommand->command().toUtf8());

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
  QNetworkRequest request(m_Url);

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  QNetworkReply *reply = m_UltraNetworkAccessManager.post(request, multiPart);
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
  int commandId = m_CommandsIdCounter++;
  m_RunningCommands.insert(commandId, abstractCommand);

  // Network request
  QNetworkRequest request(url);

  // Ignore ssl certificate
  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  QNetworkReply *reply = m_UltraNetworkAccessManager.get(request);
  reply->setProperty(PROPERTY_COMMAND_ID, commandId);

  connect(reply,
          SIGNAL(downloadProgress(qint64, qint64)),
          SLOT  (slot_QNetworkReply_downloadProgress(qint64, qint64)));
}

//-----------------------------------------------------------------------------------------------------------------------------







