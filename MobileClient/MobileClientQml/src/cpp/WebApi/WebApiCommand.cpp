/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      29.10.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "WebApiCommand.h"

// Project includes ------------------------
#include "WebApi.h"

//-----------------------------------------------------------------------------------------------------------------------------

WebApiCommand::WebApiCommand(QObject *parent) :
  QObject(parent),
  m_CommandName   (),
  m_AnswerType(JSON),
  m_Running   (false),
  m_JsonDocumentResult    (),
  m_RawResult (),
  m_Error     (WebApiError::NONE, "")
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setCommandName(const QString &command)
{
  m_CommandName = command;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::commandName() const
{
  return m_CommandName;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant WebApiCommand::requestParameter(const QString &parameterName) const
{
  return m_QMap_QueryItems.value(parameterName).second();
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setResult(const WebApiError &error,
                              const QJsonDocument &result)
{
  m_Running = false;

  m_JsonDocumentResult = result;
  m_Error = error;
  emit signal_Finished();
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant WebApiCommand::resultParameter(const QString &parameterName)
{
  return m_JsonDocumentResult.object().value(parameterName).toVariant();
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonValue WebApiCommand::resultProperty(const QString &propertyName)
{
  return m_JsonDocumentResult.object().value(propertyName);
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonArray WebApiCommand::resultArray()
{
  return m_JsonDocumentResult.array();
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setRawResult(const WebApiError &error,
                                 const QByteArray &result)
{
  m_Running = false;

  m_RawResult = result;
  emit signal_Finished();
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------

QByteArray WebApiCommand::rawResult()
{
  return m_RawResult;
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setProgress(qint64 progress,
                                qint64 total)
{
  emit signal_Progress(progress,
                       total);
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setAnswerType(WebApiCommand::ANSWER_TYPE type)
{
  m_AnswerType = type;
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApiError WebApiCommand::postRequest(QList<QueryItem> &qList_QueryItems,
                                       QIODevice *device)
{
  if(m_Running)
    return WebApiError(WebApiError::COMMAND_ALREADY_RUNNING);

  m_Running = true;

  m_QMap_QueryItems.clear();
  foreach (QueryItem queryItem, qList_QueryItems)
  {
    m_QMap_QueryItems.insert(queryItem.first(), queryItem);
  }


  if(device == NULL)
  {
    WebApi::instance()->postRequest(this,
                                    qList_QueryItems);
  }
  else
  {
    WebApi::instance()->multipartRequest(this,
                                         qList_QueryItems,
                                         device);
  }

  return WebApiError(WebApiError::NONE);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::requestString() const
{
  QStringList requestString;
  requestString << "Command: " << m_CommandName << " ";
  requestString << "Items: {";

  foreach(QueryItem item, m_QMap_QueryItems)
  {
      requestString << "(" << item.first() << ";" << item.second() << ")";
  }

  requestString << "}";

  return requestString.join("");
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApiError WebApiCommand::error() const
{
    return m_Error;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::errorString() const
{
    return m_Error.text();
}

//-----------------------------------------------------------------------------------------------------------------------------





