
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

void Command::setResponseParameter(const QString &responseParameterName,
                                   const QString &responseParameterString)
{
  m_QJsonOject_ResponseParameter.insert(responseParameterName,
                                        responseParameterString);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::getResponseParameter(const QString &responseParameterName,
                                   QString &responseParameterString)
{
  responseParameterString = m_QJsonOject_ResponseParameter.value(responseParameterName).toString();
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

