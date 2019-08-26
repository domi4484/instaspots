
// Files includes --------------------------
#include "Command.h"

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

