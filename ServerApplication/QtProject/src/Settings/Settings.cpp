
// File includes ---------------------------
#include "Settings.h"

// Project includes ------------------------
#include "../HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const QString Settings::_CONST::LOCATION           ("Werkstoffen");
const QString Settings::_CONST::FILENAME_EXTENSION (".json");

const QString Settings::_CONST::JSON::NAME ("Name");

const QString Settings::_CONST::JSON::VALUES::ROOT_NAME    ("Values");
const QString Settings::_CONST::JSON::VALUES::THICKNESS    ("Thickness");
const QString Settings::_CONST::JSON::VALUES::SURFACEVALUE ("SurfaceValue");
const QString Settings::_CONST::JSON::VALUES::CUTVALUE     ("CutValue");

//-----------------------------------------------------------------------------------------------------------------------------

Settings::Settings(const QFileInfo &qFileInfo) :
  m_QFileInfo(qFileInfo),
  m_QMap_Values()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void Settings::Load()
{
  if(m_QFileInfo.exists() == false)
    throw Exception(QString("File '%1' does not exist").arg(m_QFileInfo.filePath()));

  QFile qFile(m_QFileInfo.filePath());
  if(qFile.open(QIODevice::ReadOnly | QIODevice::Text)
     == false)
  {
    throw Exception(QString("Can't open '%1'").arg(m_QFileInfo.filePath()));
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
  QJsonValue qJsonValue_Name = qJsonObject.value(_CONST::JSON::NAME);

  // Name
  m_Name = qJsonValue_Name.toString();

  // Values
  m_QMap_Values.clear();
  QJsonArray jsonArray_Values = qJsonObject.value(_CONST::JSON::VALUES::ROOT_NAME).toArray();
  for(int i = 0; i < jsonArray_Values.size(); i++)
  {
    QJsonObject jsonObject_Value = jsonArray_Values.at(i).toObject();

    Value value;

    value.m_Thickness    = jsonObject_Value.value(_CONST::JSON::VALUES::THICKNESS).toDouble();
    value.m_SurfaceValue = jsonObject_Value.value(_CONST::JSON::VALUES::SURFACEVALUE).toDouble();
    value.m_CutValue     = jsonObject_Value.value(_CONST::JSON::VALUES::CUTVALUE).toDouble();

    m_QMap_Values.insert(value.m_Thickness,
                         value);
  } // for
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Settings::getName() const
{
  return m_Name;
}

//-----------------------------------------------------------------------------------------------------------------------------

QFileInfo Settings::getFileInfo() const
{
  return m_QFileInfo;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<double> Settings::getThicknessList() const
{
  return m_QMap_Values.keys();
}

//-----------------------------------------------------------------------------------------------------------------------------

double Settings::getSurfaceValue(double thickness) const
{
  return m_QMap_Values.value(thickness).m_SurfaceValue;
}

//-----------------------------------------------------------------------------------------------------------------------------

double Settings::getCutValue(double thickness) const
{
  return m_QMap_Values.value(thickness).m_CutValue;
}

//-----------------------------------------------------------------------------------------------------------------------------
