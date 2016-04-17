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

#ifndef COMMAND_UPLOAD_H
#define COMMAND_UPLOAD_H

// Project includes ------------------------
#include "WebApiError.h"
#include "WebApiCommand.h"

// Qt includes -----------------------------
#include <QGeoCoordinate>
#include <QPixmap>

class PictureUploader : public QObject
{

  Q_OBJECT

  Q_PROPERTY(QGeoCoordinate coordinate  READ coordinate WRITE setCoordinate NOTIFY signal_Coordinate_changed )

public:

  explicit PictureUploader(QObject *parent = 0);
  virtual ~PictureUploader();

  QGeoCoordinate coordinate() const;

  void setCoordinate(const QGeoCoordinate &coordinate);


public slots:

  QString lastErrorText() const { return m_LastErrorText; }

  // Getters
  bool    isNewSpot()   const;
  QString name()        const;
  QString description() const;
  bool    secretSpot()  const;

  // Setters
  void setNewSpot(bool newSpot);
  void setPictureFilePath(const QString &path);
  void setCameraPictureFilePath(const QString &path);
  void setCropPicture(const QString &source_url,
                      qreal crop_x,
                      qreal crop_y,
                      qreal crop_side);
  void setName(const QString &title);
  void setDescription(const QString &description);
  void setSecretSpot(bool secretSpot);
  void setExistingSpotId(int spotId);

  bool execute();
  void resetDefaults();

signals:

  void signal_UploadFinished(bool success);

  void signal_Coordinate_changed();

private slots:

  void slot_CommandUploadNewSpot_Finished(const WebApiError &error);
  void slot_CommandUploadPictureToSpot_Finished(const WebApiError &error);

private:

  QString m_LastErrorText;

  QString m_Command;
  QPixmap m_Pixmap;

  QGeoCoordinate m_QGeoCoordinate;

  int m_SpotId;

  QString m_Name;
  QString m_Description;
  bool    m_SecretSpot;

  WebApiCommand m_WebApiCommand_UploadNewSpot;
  WebApiCommand m_WebApiCommand_UploadPictureToSpot;

  bool uploadNewSpot();
  bool uploadPictureToSpot();
};

#endif // COMMAND_UPLOAD_H
