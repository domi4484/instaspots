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

// Qt includes -----------------------------
#include <QDebug>
#include <QBuffer>
#include <QUrl>

//-----------------------------------------------------------------------------------------------------------------------------

const QString PictureUploader::C_NEW_SPOT          ("uploadNewSpot");
const QString PictureUploader::C_PICTURE_TO_SPOT   ("uploadPictureToSpot");

const QString PictureUploader::R_PARAM_ID_SPOT     ("id_spot");
const QString PictureUploader::R_PARAM_LATITUDE    ("latitude");
const QString PictureUploader::R_PARAM_LONGITUDE   ("longitude");
const QString PictureUploader::R_PARAM_NAME        ("name");
const QString PictureUploader::R_PARAM_DESCRIPTION ("description");

const QString PictureUploader::A_PARAM_SUCCESSFUL  ("successful");

//-----------------------------------------------------------------------------------------------------------------------------

PictureUploader::PictureUploader(QObject *parent) :
  QObject (parent),
  m_LastErrorText                    (),
  m_Command                          (C_PICTURE_TO_SPOT),
  m_Pixmap                           (),
  m_Latitude                         (0),
  m_Longitude                        (0),
  m_SpotId                           (-1),
  m_Name                             (),
  m_Description                      (),
  m_WebApiCommand_UploadNewSpot      (this),
  m_WebApiCommand_UploadPictureToSpot(this)
{
  m_WebApiCommand_UploadNewSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadNewSpot.setCommand(C_NEW_SPOT);
  connect(&m_WebApiCommand_UploadNewSpot,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandUploadNewSpot_Finished(const WebApiError &)));

  m_WebApiCommand_UploadPictureToSpot.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_UploadPictureToSpot.setCommand(C_NEW_SPOT);
  connect(&m_WebApiCommand_UploadPictureToSpot,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandUploadPictureToSpot_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureUploader::setNewSpot(bool newSpot)
{
  if(newSpot)
  {
    m_Command = C_NEW_SPOT;
  }
  else
  {
    m_Command = C_PICTURE_TO_SPOT;
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

  m_Pixmap = qPixmap.copy(crop_x, crop_y, crop_x + crop_side, crop_y + crop_side);
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

void PictureUploader::setPosition(float latitude,
                                  float longitude)
{
  m_Latitude = latitude;
  m_Longitude = longitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureUploader::execute()
{
  if(m_Command == C_NEW_SPOT)
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
  qList_QueryItems.append(QueryItem(R_PARAM_LATITUDE,    QString::number(m_Latitude)));
  qList_QueryItems.append(QueryItem(R_PARAM_LONGITUDE,   QString::number(m_Longitude)));
  qList_QueryItems.append(QueryItem(R_PARAM_NAME,        m_Name));
  qList_QueryItems.append(QueryItem(R_PARAM_DESCRIPTION, m_Description));

  QBuffer *buffer = new QBuffer();
  buffer->open(QIODevice::WriteOnly);
  m_Pixmap.save(buffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  m_WebApiCommand_UploadNewSpot.postRequest(qList_QueryItems,
                                            buffer);

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
  qList_QueryItems.append(QueryItem(R_PARAM_ID_SPOT,   QString::number(m_SpotId)));
  qList_QueryItems.append(QueryItem(R_PARAM_LATITUDE,  QString::number(m_Latitude)));
  qList_QueryItems.append(QueryItem(R_PARAM_LONGITUDE, QString::number(m_Longitude)));

  QBuffer *buffer = new QBuffer();
  buffer->open(QIODevice::WriteOnly);
  m_Pixmap.save(buffer,
                "JPG",
                90); // writes pixmap into bytes in JPG format

  m_WebApiCommand_UploadPictureToSpot.postRequest(qList_QueryItems,
                                                  buffer);

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------






