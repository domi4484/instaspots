
// Files includes --------------------------
#include "Command.h"

// Library includes ------------------------
#include "HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QJsonDocument>
#include <QJsonArray>

//-----------------------------------------------------------------------------------------------------------------------------

Command::Command(const QString &name,
                 QObject *parent)
  : QObject(parent)
  , m_Name(name)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command::GetName() const
{
  return m_Name;
}

//-----------------------------------------------------------------------------------------------------------------------------

QByteArray Command::GetSendCommandData() const
{
  // Prepare JSON data
  QJsonObject qJsonObject;
  getValues_Command(qJsonObject);

  QJsonDocument qJsonDocument(qJsonObject);
  return qJsonDocument.toJson();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::SetFromReceiveResponseData(const QByteArray &data)
{
  QJsonParseError qJsonParseError;
  QJsonDocument qJsonDocument = QJsonDocument::fromJson(data,
                                                        &qJsonParseError);

  if(qJsonParseError.error != QJsonParseError::NoError)
  {
    throw Exception(QString("Can't parse command response: '%1'; Error: '%2'")
                            .arg(qJsonParseError.errorString())
                            .arg(QString::fromUtf8(data)));
  }
  QJsonObject qJsonObject = qJsonDocument.object();

  setValues_Response(qJsonObject);
}

//-----------------------------------------------------------------------------------------------------------------------------

QByteArray Command::GetSendResponseData() const
{
  // Prepare JSON data
  QJsonObject qJsonObject;
  getValues_Response(qJsonObject);

  QJsonDocument qJsonDocument(qJsonObject);
  return qJsonDocument.toJson();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::SetFromReceiveCommandData(const QByteArray &data)
{
  QJsonParseError qJsonParseError;
  QJsonDocument qJsonDocument = QJsonDocument::fromJson(data,
                                                        &qJsonParseError);

  if(qJsonParseError.error != QJsonParseError::NoError)
  {
    throw Exception(QString("Can't parse command: '%1'; Error: '%2'")
                            .arg(qJsonParseError.errorString())
                            .arg(QString::fromUtf8(data)));
  }
  QJsonObject qJsonObject = qJsonDocument.object();

  setValues_Command(qJsonObject);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::SetResponseParameters(const QJsonObject &qJsonObject)
{
  m_QJsonOject_ResponseParameter = qJsonObject;
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonObject Command::GetResponseParameters() const
{
  return m_QJsonOject_ResponseParameter;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setCommandParameter(const QString &parameterName,
                                  int parameterValue)
{
  m_QJsonOject_CommandParameter.insert(parameterName,
                                       parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setCommandParameter(const QString &parameterName,
                                  const QString &parameterValue)
{
  m_QJsonOject_CommandParameter.insert(parameterName,
                                       parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setCommandParameter(const QString &parameterName,
                                  const QDateTime &parameterValue)
{
  m_QJsonOject_CommandParameter.insert(parameterName,
                                       parameterValue.toString(Qt::ISODateWithMs));
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setCommandParameter(const QString &parameterName,
                                  const QJsonObject &parameterValue)
{
  m_QJsonOject_CommandParameter.insert(parameterName,
                                       parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

int Command::getCommandParameter(const QString &parameterName,
                                 int parameterValueDefault) const
{
  if(m_QJsonOject_CommandParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_CommandParameter.value(parameterName).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command::getCommandParameter(const QString &parameterName,
                                     const QString &parameterValueDefault) const
{
  if(m_QJsonOject_CommandParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_CommandParameter.value(parameterName).toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

QDateTime Command::getCommandParameter(const QString &parameterName,
                                       const QDateTime &parameterValueDefault) const
{
  if(m_QJsonOject_CommandParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return QDateTime::fromString(m_QJsonOject_CommandParameter.value(parameterName).toString(),
                               Qt::ISODateWithMs);
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonObject Command::getCommandParameter(const QString &parameterName,
                                         const QJsonObject &parameterValueDefault) const
{
  if(m_QJsonOject_CommandParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_CommandParameter.value(parameterName).toObject();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setResponseParameter(const QString &parameterName,
                                   int parameterValue)
{
  m_QJsonOject_ResponseParameter.insert(parameterName,
                                        parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setResponseParameter(const QString &parameterName,
                                   const QString &parameterValue)
{
  m_QJsonOject_ResponseParameter.insert(parameterName,
                                        parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setResponseParameter(const QString &parameterName,
                                   const QDateTime &parameterValue)
{
  m_QJsonOject_ResponseParameter.insert(parameterName,
                                        parameterValue.toString(Qt::ISODateWithMs));
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setResponseParameter(const QString &parameterName,
                                   const QJsonObject &parameterValue)
{
  m_QJsonOject_ResponseParameter.insert(parameterName,
                                        parameterValue);
}

//-----------------------------------------------------------------------------------------------------------------------------

int Command::getResponseParameter(const QString &parameterName,
                                  int parameterValueDefault) const
{
  if(m_QJsonOject_ResponseParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_ResponseParameter.value(parameterName).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command::getResponseParameter(const QString &parameterName,
                                      const QString &parameterValueDefault) const
{
  if(m_QJsonOject_ResponseParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_ResponseParameter.value(parameterName).toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

QDateTime Command::getResponseParameter(const QString &parameterName,
                                        const QDateTime &parameterValueDefault) const
{
  if(m_QJsonOject_ResponseParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return QDateTime::fromString(m_QJsonOject_ResponseParameter.value(parameterName).toString(),
                               Qt::ISODateWithMs);
}

//-----------------------------------------------------------------------------------------------------------------------------

QJsonObject Command::getResponseParameter(const QString &parameterName,
                                          const QJsonObject &parameterValueDefault) const
{
  if(m_QJsonOject_ResponseParameter.value(parameterName).isUndefined() == true)
    return parameterValueDefault;

  return m_QJsonOject_ResponseParameter.value(parameterName).toObject();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::getValues_Command(QJsonObject &qJsonObject) const
{
  QJsonObject qJsonObject_Command;
  qJsonObject_Command.insert("Name", m_Name);

  qJsonObject.insert("Command", qJsonObject_Command);

  qJsonObject.insert("CommandParameters", m_QJsonOject_CommandParameter);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setValues_Response(const QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_Command = qJsonObject.value("Command").toObject();
  if(qJsonObject_Command.isEmpty())
    return;

  m_Name = qJsonObject_Command.value("Name").toString();

  m_QJsonOject_ResponseParameter = qJsonObject_Command.value("ResponseParameters").toObject();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::getValues_Response(QJsonObject &qJsonObject) const
{
  QJsonObject qJsonObject_Command;
  qJsonObject_Command.insert("Name", m_Name);

  qJsonObject.insert("Command", qJsonObject_Command);

  qJsonObject.insert("ResponseParameters", m_QJsonOject_ResponseParameter);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setValues_Command(const QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_Command = qJsonObject.value("Command").toObject();
  if(qJsonObject_Command.isEmpty())
    return;

  m_Name = qJsonObject_Command.value("Name").toString();

  m_QJsonOject_CommandParameter = qJsonObject_Command.value("CommandParameters").toObject();
}

