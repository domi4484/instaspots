
// Files includes --------------------------
#include "Command.h"

// Library includes ------------------------
#include "HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QJsonDocument>
#include <QJsonArray>

//-----------------------------------------------------------------------------------------------------------------------------

Command::Command(const QString &name)
  : m_Name(name)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QByteArray Command::GetSendCommandData() const
{
  // Prepare JSON data
  QJsonObject qJsonObject;
  getValues_Command(qJsonObject);
  getValues_CommandParameters(qJsonObject);

  QJsonDocument qJsonDocument(qJsonObject);
  return qJsonDocument.toJson();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::SetFromReceiveResponseData(const QByteArray &data)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QByteArray Command::GetSendResponseData() const
{

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

void Command::getValues_Command(QJsonObject &qJsonObject) const
{
  QJsonObject qJsonObject_Command;
  qJsonObject_Command.insert("Name", m_Name);

  qJsonObject.insert("Command", qJsonObject_Command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::getValues_CommandParameters(QJsonObject &qJsonObject) const
{
  QJsonObject qJsonObject_Command;
//  qJsonObject_Command.insert("Name", m_Name);

  qJsonObject.insert("CommandParameters", qJsonObject_Command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command::setValues_Command(const QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_Command = qJsonObject.value("Command").toObject();
  if(qJsonObject_Command.isEmpty())
    return;

  m_Name = qJsonObject_Command.value("Name").toString();
}

