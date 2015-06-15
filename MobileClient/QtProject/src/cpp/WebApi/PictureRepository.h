/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.06.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef PICTUREREPOSITORY_H
#define PICTUREREPOSITORY_H

// Project includes ------------------------
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QObject>
#include <QMap>

// Forward declarations --------------------
class Picture;

class PictureRepository : public QObject
{

  Q_OBJECT

public:

  explicit PictureRepository(QObject *parent = 0);
  ~PictureRepository();

  QList<Picture *> getPictures(int requestId);

  int getBy_SpotId(int spotId);

signals:

  void signal_DataReady(int requestId,
                        bool success);

private slots:

  void slot_Command_Finished(const WebApiError &error);

private:

  static const char* PROPERTY_REQUEST_ID;

  int m_RequestId;

  QMap<int, Picture *> m_QMap_Pictures;

  QMap<int, QList<Picture *> > m_QMap_Results;
};

#endif // PICTUREREPOSITORY_H
