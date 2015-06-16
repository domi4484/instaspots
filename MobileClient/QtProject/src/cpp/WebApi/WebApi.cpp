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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QScriptEngine>
#include <QScriptValueList>
#include <QUrlQuery>
#include <QHttpMultiPart>

//-----------------------------------------------------------------------------------------------------------------------------

// Location of the web service
const QString WebApi::URL_DEVELOPMENT ("http://localhost/Symfony/web/app_dev.php/spots/webservice");
const QString WebApi::URL_PRODUCTION  ("http://spots.lowerclassclothing.com/web/spots/webservice");


const QString WebApi::CONST::GENERAL_PARAMS::COMMAND    ("command");
const QString WebApi::CONST::GENERAL_PARAMS::ERROR      ("error");

const char *WebApi::PROPERTY_COMMAND_ID = "command_id";

const QString WebApi::C_LOGIN            ("login");
const QString WebApi::C_LOGOUT           ("logout");
const QString WebApi::C_REGISTER         ("register");
const QString WebApi::C_CANREGISTER      ("canregister");
const QString WebApi::C_GET_NEARBY_SPOTS ("getNearbySpots");
const QString WebApi::C_GET_PICTURES     ("getPictures");
const QString WebApi::C_GET_NEWS         ("getNews");

const QString WebApi::R_PARAM_USERNAME ("username");
const QString WebApi::R_PARAM_PASSWORD ("password");
const QString WebApi::R_PARAM_EMAIL    ("email");

const QString WebApi::R_PARAM_SPOT_ID ("id_spot");

const QString WebApi::R_PARAM_LATITUDE  ("latitude");
const QString WebApi::R_PARAM_LONGITUDE ("longitude");

const QString WebApi::A_PARAM_AUTHENTICATION ("authentication");
const QString WebApi::A_PARAM_REGISTERED     ("registered");

const QString WebApi::A_ARRAY_SPOTS ("spots");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_ID            ("id");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_NAME          ("name");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_DESCRIPTION   ("description");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_LATITUDE      ("latitude");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_LONGITUDE     ("longitude");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_DISTANCE      ("distance");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1 ("pictureUrl1");
const QString WebApi::A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2 ("pictureUrl2");

const QString WebApi::A_ARRAY_PICTURES ("pictures");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_ID          ("id");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_ID_USER     ("id_user");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_ID_SPOT     ("id_spot");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_URL         ("url");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_USERNAME    ("username");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_NAME        ("name");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_DESCRIPTION ("description");
const QString WebApi::A_ARRAY_PICTURES_ELEMENT_CREATED     ("created");


//-----------------------------------------------------------------------------------------------------------------------------

WebApi *WebApi::s_Instance(NULL);

//-----------------------------------------------------------------------------------------------------------------------------

WebApi::WebApi() :
  QObject(),
  m_Url(URL_PRODUCTION),
  m_QNetworkAccessManager(),
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
                       QScriptValue());
    return;
  }

  if(   received != 0
     && total    != 0)
  {
    Logger::debug(QString("%1/%2").arg(received)
                  .arg(total));
    command->setProgress(received,
                         total);
    return;
  }

  // Delete reply
  reply->deleteLater();

  // Received data
  QString data = reply->readAll();
  Logger::debug(QString("Received: %1").arg(data));

  // Parse received JSON
  QScriptEngine engine;
  QScriptValue result = engine.evaluate(QString("(%1)").arg(data));
  QScriptValue qScriptValue_Error = result.property(CONST::GENERAL_PARAMS::ERROR);

  // Answer not valid. Probably a script syntax error
  if(qScriptValue_Error.isValid() == false)
  {
    Logger::error("Script error");
    command->setResult(WebApiError(WebApiError::SERVER),
                       result);
    return;
  }

  // Command error
  QString errorText = qScriptValue_Error.toString();
  if(errorText.isEmpty() == false)
  {
    Logger::error("Error: " + errorText);
    command->setResult(WebApiError(WebApiError::COMMAND,
                                   errorText),
                       result);
    return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     result);
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
                       QScriptValue());
    return;
  }

  if(received != total)
  {
    Logger::debug(QString("%1/%2").arg(received)
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
    Logger::error(QString("Network error %1 (%2)").arg(replyNetworkError)
                                                  .arg(replyNetworkErrorString));
    command->setResult(WebApiError(WebApiError::NETWORK),
                       QScriptValue());
    return;
  }

  // Received data
  QString data = reply->readAll();
  Logger::debug(QString("Received: %1").arg(data));

  // Parse received JSON
  QScriptEngine engine;
  QScriptValue result = engine.evaluate(QString("(%1)").arg(data));
  QScriptValue qScriptValue_Error = result.property(CONST::GENERAL_PARAMS::ERROR);

  // Answer not valid. Probably a script syntax error
  if(qScriptValue_Error.isValid() == false)
  {
    Logger::error("Script error");
    command->setResult(WebApiError(WebApiError::SERVER),
                       result);
    return;
  }

  // Command error
  QString errorText = qScriptValue_Error.toString();
  if(errorText.isEmpty() == false)
  {
    Logger::error("Error: " + errorText);
    command->setResult(WebApiError(WebApiError::COMMAND,
                                   errorText),
                       result);
    return;
  }

  command->setResult(WebApiError(WebApiError::NONE),
                     result);
}

//----------------------------------------------------------------------------------------------------------------------------

void WebApi::postRequest(WebApiCommand *abstractCommand,
                         const QList<QueryItem> &qList_QueryItems)
{
  int commandId = m_CommandsIdCounter++;
  m_RunningCommands.insert(commandId, abstractCommand);

  QNetworkRequest request(m_Url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem(CONST::GENERAL_PARAMS::COMMAND,    abstractCommand->command());
  foreach(QueryItem item, qList_QueryItems)
  {
    params.addQueryItem(item.first(), item.second());
  }

  QNetworkReply *reply = m_QNetworkAccessManager.post(request, params.query(QUrl::FullyEncoded).toUtf8());
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

  QNetworkRequest request(m_Url);

  QNetworkReply *reply = m_QNetworkAccessManager.post(request, multiPart);
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

  QNetworkRequest request(url);

  QNetworkReply *reply = m_QNetworkAccessManager.get(request);
  reply->setProperty(PROPERTY_COMMAND_ID, commandId);

  connect(reply,
          SIGNAL(downloadProgress(qint64, qint64)),
          SLOT  (slot_QNetworkReply_downloadProgress(qint64, qint64)));
}

//-----------------------------------------------------------------------------------------------------------------------------







