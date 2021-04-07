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

WebApiCommand::WebApiCommand(QObject *parent)
  : QObject(parent)
  , mCommandName()
  , mRequestType(RequestTypePost)
  , mAnswerType(AnswerTypeJSON)
  , m_Running(false)
  , m_JsonDocumentResult()
  , m_RawResult()
  , m_Error(WebApiError::NONE, "")
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setCommandName(const QString &command)
{
  mCommandName = command;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::commandName() const
{
  return mCommandName;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant WebApiCommand::requestParameter(const QString &parameterName) const
{
  return mQMapQueryItems.value(parameterName).second();
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

void WebApiCommand::setRequestType(EnumRequestType requestType)
{
  mRequestType = requestType;
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApiCommand::EnumRequestType WebApiCommand::requestType() const
{
  return mRequestType;
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setAnswerType(EnumAnswerType type)
{
  mAnswerType = type;
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApiCommand::EnumAnswerType WebApiCommand::answerType() const
{
  return mAnswerType;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool WebApiCommand::isRunning()
{
  return m_Running;
}

//-----------------------------------------------------------------------------------------------------------------------------

WebApiError WebApiCommand::sendRequest(QList<QueryItem> &qList_QueryItems,
                                       QIODevice *device)
{
  if(m_Running)
    return WebApiError(WebApiError::COMMAND_ALREADY_RUNNING);

  m_Running = true;

  mQMapQueryItems.clear();
  foreach (QueryItem queryItem, qList_QueryItems)
  {
    mQMapQueryItems.insert(queryItem.first(), queryItem);
  }

  if(device == NULL)
  {
    WebApi::instance()->sendRequest(this,
                                    qList_QueryItems);
  }
  else
  {
    WebApi::instance()->sendMultipartRequest(this,
                                             qList_QueryItems,
                                             device);
  }

  return WebApiError(WebApiError::NONE);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::requestString() const
{
  QStringList requestString;
  requestString << "Command: " << mCommandName << " ";
  requestString << "Items: {";

  foreach(QueryItem item, mQMapQueryItems)
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





