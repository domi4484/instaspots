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

// Qt includes -----------------------------
#include <QObject>

// Forward declarations --------------------
class Picture;

class PictureRepository : public QObject
{

  Q_OBJECT

public:

  explicit PictureRepository(QObject *parent = 0);
  ~PictureRepository();


  QList<Picture *> getPictures();

  void getBy_SpotId(int spotId);


signals:

public slots:
};

#endif // PICTUREREPOSITORY_H
