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
#include <QBuffer>
#include <QUrl>

//-----------------------------------------------------------------------------------------------------------------------------

PictureUploader::PictureUploader(QObject *parent) :
  QObject (parent),
  m_LastErrorText                    (),
  m_Command                          (WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT),
  m_Pixmap                           (),
  m_QGeoCoordinate                   (),
  m_SpotId                           (-1),
  m_Name                             (),
  m_Description                      (),
  m_SecretSpot                       (false),
  m_WebApiCommand_UploadNewSpot      (this),
  m_WebApiCommand_UploadPictureToSpot(this)
{
  m_WebApiCommand_UploadNewSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadNewSpot.setCommand(WebApi::COMMAND::UPLOAD_NEW_SPOT);
  connect(&m_WebApiCommand_UploadNewSpot,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandUploadNewSpot_Finished(const WebApiError &)));

  m_WebApiCommand_UploadPictureToSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadPictureToSpot.setCommand(WebApi::COMMAND::UPLOAD_PICTURE_TO_SPOT);
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

void PictureUploader::setPictureFilePath(const QString &path)
{
  m_Pixmap = path;
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setCameraPictureFilePath(const QString &path)
{
  QPixmap qPixmap(path);
  if(qPixmap.isNull())
  {
    Logger::error("Invalid QPixmap");
    m_Pixmap = QPixmap();
    return;
  }

  if(qPixmap.width() > qPixmap.height())
  {
    double diff = (qPixmap.width() - qPixmap.height());
    m_Pixmap = qPixmap.copy(diff/2.0, 0, qPixmap.width() - diff, qPixmap.height());
    m_Pixmap = m_Pixmap.scaled(640, 640,
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);
  }
  else if(qPixmap.height() > qPixmap.width())
  {
    double diff = (qPixmap.height() - qPixmap.width());
    m_Pixmap = qPixmap.copy(0, diff/2.0, qPixmap.width(), qPixmap.height() - diff);
    m_Pixmap = m_Pixmap.scaled(640, 640,
                              Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
  }
  else
  {
    m_Pixmap = qPixmap.scaled(640, 640,
                              Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setCropPicture(const QString &source_url,
                                     qreal crop_x,
                                     qreal crop_y,
                                     qreal crop_side)
{
  QPixmap qPixmap(QUrl(source_url).toLocalFile());
  if(qPixmap.isNull())
  {
    Logger::error("Invalid QPixmap");
    m_Pixmap = QPixmap();
    return;
  }

  Logger::trace(QString("Crop picture: x: %1, y: %2, side: %3").arg(crop_x)
                                                               .arg(crop_y)
                                                               .arg(crop_side));

  m_Pixmap = qPixmap.copy(crop_x,
                          crop_y,
                          crop_side,
                          crop_side);
  m_Pixmap = m_Pixmap.scaled(640, 640,
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation);
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

QString PictureUploader::rotatePicture(const QString &source_url,
                                       int angle)
{
  Logger::info(source_url);
  QString localFilename = QUrl(source_url).toLocalFile();
  Logger::info(localFilename);

  QPixmap qPixmap(localFilename);
  if(qPixmap.isNull())
  {
    Logger::error("Invalid QPixmap");
    return QString();
  }

  QMatrix qMatrix_Rotation;
  qMatrix_Rotation.rotate(angle);
  qPixmap = qPixmap.transformed(qMatrix_Rotation);

//  if(localFilename.endsWith("-rotated.jpg"))
  {
    qPixmap.save(localFilename);
    return source_url;
  }

  QString newFilename = localFilename;
  newFilename = newFilename.replace(".jpg", "-rotated.jpg");
  qPixmap.save(newFilename);

  QString newUrl = source_url;
  newUrl.replace(".jpg", "-rotated.jpg");
  return newUrl;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::execute()
{
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
  if(m_Name.isEmpty())
  {
    m_LastErrorText = tr("Spot name can't be empty");
    Logger::error(m_LastErrorText);
    return false;
  }

  if(m_Pixmap.isNull())
  {
    m_LastErrorText = tr("Invalid picture");
    Logger::error(m_LastErrorText);
    return false;
  }

  if(m_Pixmap.size() != QSize(640, 640))
  {
    m_LastErrorText = tr("Invalid picture format");
    Logger::error(m_LastErrorText);
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_NAME,        m_Name));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_DESCRIPTION, m_Description));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_SECRET_SPOT, QString::number(m_SecretSpot)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LATITUDE,    QString::number(m_QGeoCoordinate.latitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::SPOT_LONGITUDE,   QString::number(m_QGeoCoordinate.longitude())));

  QBuffer *buffer = new QBuffer();
  buffer->open(QIODevice::WriteOnly);
  m_Pixmap.save(buffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  WebApiError error = m_WebApiCommand_UploadNewSpot.postRequest(qList_QueryItems,
                                                                buffer);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::uploadPictureToSpot()
{
  if(m_SpotId <= 0)
  {
    m_LastErrorText = tr("Invalid spotId (%1)").arg(m_SpotId);
    Logger::error(m_LastErrorText);
    return false;
  }

  if(m_Pixmap.isNull())
  {
    m_LastErrorText = tr("Invalid picture");
    Logger::error(m_LastErrorText);
    return false;
  }

  if(m_Pixmap.size() != QSize(640, 640))
  {
    m_LastErrorText = tr("Invalid picture format");
    Logger::error(m_LastErrorText);
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_SPOT_ID,   QString::number(m_SpotId)));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_LATITUDE,  QString::number(m_QGeoCoordinate.latitude())));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::PICTURE_LONGITUDE, QString::number(m_QGeoCoordinate.longitude())));

  QBuffer *buffer = new QBuffer();
  buffer->open(QIODevice::WriteOnly);
  m_Pixmap.save(buffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  WebApiError error = m_WebApiCommand_UploadPictureToSpot.postRequest(qList_QueryItems,
                                                                      buffer);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------






