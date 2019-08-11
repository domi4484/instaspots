
// File includes ---------------------------
#include "Settings.h"

// Project includes ------------------------
#include "../HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Settings::_CONST::FILENAME_EXTENSION (".json");

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Settings() :
  m_QFileInfo(),
  m_Values()
{
  createDefault_Values();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::Load(const QFileInfo &qFileInfo)
{
  m_QFileInfo = qFileInfo;

  if(m_QFileInfo.exists() == false)
    throw Exception(QString("File '%1' does not exist").arg(m_QFileInfo.filePath()));

  QFile qFile(m_QFileInfo.filePath());
  if(qFile.open(QIODevice::ReadOnly | QIODevice::Text)
     == false)
  {
    throw Exception(QString("Can't open '%1' for reading").arg(m_QFileInfo.filePath()));
  }

  QString fileContent = qFile.readAll();
  qFile.close();

  QJsonParseError qJsonParseError;
  QJsonDocument qJsonDocument = QJsonDocument::fromJson(fileContent.toUtf8(),
                                                        &qJsonParseError);

  if(qJsonParseError.error != QJsonParseError::NoError)
  {
    throw Exception(QString("Can't parse '%1' -> %2").arg(m_QFileInfo.filePath())
                                                     .arg(qJsonParseError.errorString()));
  }
  QJsonObject qJsonObject = qJsonDocument.object();

  load_Values(qJsonObject);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::Write(const QString &filename)
{
  // Prepare JSON data
  QByteArray qByteArray_WriteData;
  {
    QJsonObject qJsonObject;
    write_Values(qJsonObject);

    QJsonDocument qJsonDocument(qJsonObject);
    qByteArray_WriteData = qJsonDocument.toJson();
  }

  QFile qFile(m_QFileInfo.filePath());
  if(filename.isEmpty() == false)
    qFile.setFileName(filename);

  if(qFile.open(QIODevice::WriteOnly | QIODevice::Text)
     == false)
  {
    throw Exception(QString("Can't open '%1' for writing").arg(qFile.fileName()));
  }

  qFile.write(qByteArray_WriteData);
  qFile.close();
}

//-----------------------------------------------------------------------------------------------------------------------------

QFileInfo Settings::getFileInfo() const
{
  return m_QFileInfo;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::Get_Values_General_ApplicationName() const
{
  return m_Values.general.m_ApplicationName;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::Get_Values_General_ApplicationVersion() const
{
  return m_Values.general.m_ApplicationVersion;
}

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Values::Logger::Enum_LogLevel Settings::Get_Values_Logger_LogLevel() const
{
  return m_Values.logger.m_LogLevel;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::createDefault_Values()
{
  createDefault_Values_General();
  createDefault_Values_Logger();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::createDefault_Values_General()
{
  m_Values.general.m_ApplicationName = QCoreApplication::applicationName();
  m_Values.general.m_ApplicationVersion = QCoreApplication::applicationVersion();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::createDefault_Values_Logger()
{
  m_Values.logger.m_LogLevel = Values::Logger::LogLevel_Debug;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::load_Values(const QJsonObject &qJsonObject)
{
  load_Values_General(qJsonObject);
  load_Values_Logger(qJsonObject);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::load_Values_General(const QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_General = qJsonObject.value("General").toObject();

  m_Values.general.m_ApplicationName = qJsonObject_General.value("ApplicationName").toString();
  m_Values.general.m_ApplicationVersion = qJsonObject_General.value("ApplicationVersion").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::load_Values_Logger(const QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_General = qJsonObject.value("Logger").toObject();

  m_Values.general.m_ApplicationName = (Values::Logger::Enum_LogLevel)qJsonObject_General.value("LogLevel").toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::write_Values(QJsonObject &qJsonObject)
{
  write_Values_General(qJsonObject);
  write_Values_Logger(qJsonObject);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::write_Values_General(QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_General;
  qJsonObject_General.insert("ApplicationName", m_Values.general.m_ApplicationName);
  qJsonObject_General.insert("ApplicationVersion", m_Values.general.m_ApplicationVersion);

  qJsonObject.insert("General", qJsonObject_General);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::write_Values_Logger(QJsonObject &qJsonObject)
{
  QJsonObject qJsonObject_Logger;
  qJsonObject_Logger.insert("LogLevel", m_Values.logger.m_LogLevel);

  qJsonObject.insert("Logger", qJsonObject_Logger);
}

//-----------------------------------------------------------------------------------------------------------------------------
