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

// Qt inlcudes -----------------------------
#include <QApplication>

//-----------------------------------------------------------------------------------------------------------------------------

WebApiCommand::WebApiCommand(QObject *parent) :
  QObject(parent),
  m_Command   (),
  m_AnswerType(JSON),
  m_Running   (false),
  m_Result    (),
  m_RawResult (),
  m_Error     (WebApiError::NONE, "")
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setCommand(const QString &command)
{
  m_Command = command;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::command() const
{
  return m_Command;
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setResult(const WebApiError &error,
                              const QJsonObject &result)
{
  m_Running = false;

  m_Result = result;
  m_Error = error;
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant WebApiCommand::resultParameter(const QString &parameterName)
{
  return m_Result.value(parameterName).toVariant();
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonValue WebApiCommand::resultProperty(const QString &propertyName)
{
  return m_Result.value(propertyName);
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonArray WebApiCommand::resultArray(const QString &arrayName)
{
  return m_Result.value(arrayName).toArray();
}

//-----------------------------------------------------------------------------------------------------------------------------

void WebApiCommand::setRawResult(const WebApiError &error,
                                 const QByteArray &result)
{
  m_Running = false;

  m_RawResult = result;
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
  {
    return WebApiError(WebApiError::COMMAND_ALREADY_RUNNING);
  }

  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_APPLICATION_VERSION,
                                    QApplication::applicationVersion()));

  m_Running = true;

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

WebApiError WebApiCommand::error()
{
    return m_Error;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiCommand::errorString()
{
    return m_Error.text();
}

//-----------------------------------------------------------------------------------------------------------------------------





