/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.03.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef PICTURECACHER_H
#define PICTURECACHER_H

// Qt imports ------------------------------
#include <QObject>
#include <QMap>

class PictureCacher : public QObject
{
  Q_OBJECT
public:
  explicit PictureCacher(QObject *parent = 0);
  ~PictureCacher();

signals:

public slots:

  bool isPictureCached(const QString &url);
  QString getThumb(const QString &url);

private:

  QMap<QString, QString> m_PictureUrl_Picture;

};

#endif // PICTURECACHER_H
