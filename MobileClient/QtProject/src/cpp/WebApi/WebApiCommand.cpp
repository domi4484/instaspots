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
  m_Command(),
  m_AnswerType(JSON),
  m_Running(false),
  m_Result(),
  m_RawResult ()
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
                              const QScriptValue &result)
{
  m_Running = false;

  m_Result = result;
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant WebApiCommand::resultParameter(const QString &parameterName)
{
  return m_Result.property(parameterName).toVariant();
}

//-----------------------------------------------------------------------------------------------------------------------------

QScriptValue WebApiCommand::resultProperty(const QString &propertyName)
{
  return m_Result.property(propertyName);
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

WebApiError WebApiCommand::postRequest(const QList<QueryItem> &queryItems,
                                       QIODevice *device)
{
  if(m_Running)
  {
    return WebApiError(WebApiError::COMMAND_ALREADY_RUNNING);
  }

  m_Running = true;

  if(device == NULL)
  {
    WebApi::instance()->postRequest(this,
                                    queryItems);
  }
  else
  {
    WebApi::instance()->multipartRequest(this,
                                         queryItems,
                                         device);
  }

  return WebApiError(WebApiError::NONE);
}

//-----------------------------------------------------------------------------------------------------------------------------





