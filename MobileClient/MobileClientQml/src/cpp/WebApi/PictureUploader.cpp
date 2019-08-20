/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      03.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "PictureUploader.h"

// Project includes ------------------------
#include "../HelperClasses/Logger.h"
#include "../WebApi/WebApi.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QUrl>

//-----------------------------------------------------------------------------------------------------------------------------

PictureUploader::PictureUploader(QObject *parent)
  : QObject (parent)
  , m_LastErrorText                    ()
  , m_Command                          (WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT)
  , m_SourcePictureFilename            ()
  , m_QPixmap                          ()
  , m_QGeoCoordinate                   ()
  , m_SpotId                           (-1)
  , m_Name                             ()
  , m_Description                      ()
  , m_SecretSpot                       (false)
  , m_QStringList_Tags                 ()
  , m_WebApiCommand_UploadNewSpot      (this)
  , m_WebApiCommand_UploadPictureToSpot(this)
{
  m_WebApiCommand_UploadNewSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadNewSpot.setCommandName(WebApi::COMMAND::UPLOAD_NEW_SPOT);
  connect(&m_WebApiCommand_UploadNewSpot,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandUploadNewSpot_Finished(const WebApiError &)));

  m_WebApiCommand_UploadPictureToSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadPictureToSpot.setCommandName(WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT);
  connect(&m_WebApiCommand_UploadPictureToSpot,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandUploadPictureToSpot_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

PictureUploader::~PictureUploader()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QGeoCoordinate PictureUploader::coordinate() const
{
    return m_QGeoCoordinate;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setCoordinate(const QGeoCoordinate &coordinate)
{
  m_QGeoCoordinate = coordinate;

  emit signal_Coordinate_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::isNewSpot() const
{
  return m_Command == WebApi::COMMAND::UPLOAD_NEW_SPOT;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString PictureUploader::name() const
{
  return m_Name;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString PictureUploader::description() const
{
  return m_Description;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::secretSpot() const
{
  return m_SecretSpot;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString PictureUploader::sourcePictureFilename() const
{
  return QUrl::fromLocalFile(m_SourcePictureFilename).toString();

}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setNewSpot(bool newSpot)
{
  if(newSpot)
  {
    m_Command = WebApi::COMMAND::UPLOAD_NEW_SPOT;
  }
  else
  {
    m_Command = WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::setSourcePictureFileUrl(const QString &url)
{
  // Url to filename
  m_SourcePictureFilename = QUrl(url).toLocalFile();

  // Open image as pixmap
  QPixmap qPixmap(m_SourcePictureFilename);
  if(qPixmap.isNull())
  {
    m_LastErrorText = QString("Invalid picture '%1'").arg(m_SourcePictureFilename);
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::setCameraPictureFilename(const QString &filename)
{
  m_SourcePictureFilename = filename;

  // Open image as pixmap
  QPixmap qPixmap(filename);
  if(qPixmap.isNull())
  {
    m_LastErrorText = QString("Invalid picture '%1'").arg(filename);
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::setCropPicture(qreal crop_x,
                                     qreal crop_y,
                                     qreal crop_side)
{ 
  QPixmap qPixmap(m_SourcePictureFilename);
  if(qPixmap.isNull())
  {
    m_QPixmap = QPixmap();

    m_LastErrorText = tr("Invalid picture");
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  Logger::trace(QString("PictureUploader::%1() Crop picture: x: %2, y: %3, side: %4").arg(__FUNCTION__)
                                                                                     .arg(crop_x)
                                                                                     .arg(crop_y)
                                                                                     .arg(crop_side));

  m_QPixmap = qPixmap.copy(crop_x,
                          crop_y,
                          crop_side,
                          crop_side);
  m_QPixmap = m_QPixmap.scaled(640, 640,
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation);

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setName(const QString &name)
{
  m_Name = name;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setDescription(const QString &description)
{
  m_Description = description;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setSecretSpot(bool secretSpot)
{
  m_SecretSpot = secretSpot;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setExistingSpotId(int spotId)
{
  m_SpotId = spotId;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::addTag(const QString &tagName)
{
  m_QStringList_Tags.append(tagName);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::rotatePicture(int angle)
{
  // Open image as pixmap
  QPixmap qPixmap(m_SourcePictureFilename);
  if(qPixmap.isNull())
  {
    m_LastErrorText = QString("Invalid picture '%1'").arg(m_SourcePictureFilename);
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  // Transform pixmap with rotation matrix
  QMatrix qMatrix_Rotation;
  qMatrix_Rotation.rotate(angle);
  qPixmap = qPixmap.transformed(qMatrix_Rotation);

  // Temporary image file
  QString temporaryFilename = QFileInfo(QDir::tempPath(),
                                        "temporaryUploadPicture.jpg").filePath();

  // Save rotated image
  if(qPixmap.save(temporaryFilename) == false)
  {
    m_LastErrorText = QString("Can't save temporary picture '%1'").arg(temporaryFilename);
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  m_SourcePictureFilename = temporaryFilename;
  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::execute()
{
  if(m_QPixmap.isNull())
  {
    m_LastErrorText = tr("Invalid picture");
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  if(m_QPixmap.size() != QSize(640, 640))
  {
    m_LastErrorText = tr("Invalid picture format");
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  if(m_Command == WebApi::COMMAND::UPLOAD_NEW_SPOT)
  {
    return uploadNewSpot();
  }
  else
  {
    return uploadPictureToSpot();
  }

  return false;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::resetDefaults()
{
  m_QGeoCoordinate.setLatitude (0.0);
  m_QGeoCoordinate.setLongitude(0.0);
  m_SpotId      = -1;
  m_Name        = "";
  m_Description = "";
  m_SecretSpot  = false;
  m_QStringList_Tags.clear();

  emit signal_Coordinate_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::slot_CommandUploadNewSpot_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    emit signal_UploadFinished(false);
    return;
  }

  m_LastErrorText = "";
  emit signal_UploadFinished(true);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::slot_CommandUploadPictureToSpot_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    emit signal_UploadFinished(false);
    return;
  }

  m_LastErrorText = "";
  emit signal_UploadFinished(true);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::uploadNewSpot()
{
  // Spot name check
  if(m_Name.isEmpty())
  {
    m_LastErrorText = tr("Spot name can't be empty");
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  Logger::info(QString("PictureUploader::%1() %2=%3; %4=%5; coordinate=(%6:%7)").arg(__FUNCTION__)
                                                                                .arg(WebApi::PARAMETER::SPOT_NAME)
                                                                                .arg(m_Name)
                                                                                .arg(WebApi::PARAMETER::SPOT_SECRET_SPOT)
                                                                                .arg(m_SecretSpot)
                                                                                .arg(m_QGeoCoordinate.latitude())
                                                                                .arg(m_QGeoCoordinate.longitude()));

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_NAME,        m_Name));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_DESCRIPTION, m_Description));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_SECRET_SPOT, QString::number(m_SecretSpot)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LATITUDE,    QString::number(m_QGeoCoordinate.latitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LONGITUDE,   QString::number(m_QGeoCoordinate.longitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_TAGS,        m_QStringList_Tags.join(";")));

  // Prepare image buffer
  QBuffer *qBuffer = new QBuffer();
  qBuffer->open(QIODevice::WriteOnly);
  m_QPixmap.save(qBuffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  // Post request
  WebApiError error = m_WebApiCommand_UploadNewSpot.postRequest(qList_QueryItems,
                                                                qBuffer);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::uploadPictureToSpot()
{
  // Spot id check
  if(m_SpotId <= 0)
  {
    m_LastErrorText = tr("Invalid spotId (%1)").arg(m_SpotId);
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  Logger::info(QString("PictureUploader::%1() %2=%3; coordinate=(%6:%7)").arg(__FUNCTION__)
                                                                         .arg(WebApi::PARAMETER::SPOT_NAME)
                                                                         .arg(m_Name)
                                                                         .arg(m_QGeoCoordinate.latitude())
                                                                         .arg(m_QGeoCoordinate.longitude()));

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_SPOT_ID,   QString::number(m_SpotId)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_LATITUDE,  QString::number(m_QGeoCoordinate.latitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_LONGITUDE, QString::number(m_QGeoCoordinate.longitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_TAGS,         m_QStringList_Tags.join(";")));

  // Prepare image buffer
  QBuffer *qBuffer = new QBuffer();
  qBuffer->open(QIODevice::WriteOnly);
  m_QPixmap.save(qBuffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  // Post request
  WebApiError error = m_WebApiCommand_UploadPictureToSpot.postRequest(qList_QueryItems,
                                                                      qBuffer);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    Logger::error(QString("PictureUploader::%1() %2").arg(__FUNCTION__)
                                                     .arg(m_LastErrorText));
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------






