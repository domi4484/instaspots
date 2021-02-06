/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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

  static void instanziate();
  static void destroy();

  static PictureRepository *instance() { return s_PictureRepository; }

  int getNewRequestId();

public slots:

  QList<Picture *> getPictures(int requestId);

  Picture *getBy_PictureId(int pictureId);

  Picture *getAdd_Picture(int pictureId);

  void getBy_PictureId(int requestId, int pictureId);
  void getBy_SpotId(int requestId, int spotId);
  void getBy_UserId(int requestId, int userId);
  void getBy_Newest(int requestId);

  void likePicture(int pictureId);
  void unlikePicture(int pictureId);

  void removePicture(int pictureId);

signals:

  void signal_DataReady(int requestId,
                        bool success);
  void signal_PictureRemoved(Picture *picture);

private slots:

  void slot_Command_Finished(const WebApiError &error);
  void slot_Command_RemovePicture_Finished(const WebApiError &error);

private:

  static const char* PROPERTY_REQUEST_ID;

  static PictureRepository *s_PictureRepository;

  int m_RequestId;

  QMap<int, Picture *> m_QMap_Pictures;

  QMap<int, QList<Picture *> > m_QMap_Results;

};

#endif // PICTUREREPOSITORY_H
