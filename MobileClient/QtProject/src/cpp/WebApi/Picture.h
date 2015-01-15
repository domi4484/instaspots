/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      05.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef PICTURE_H
#define PICTURE_H

// Project includes ------------------------
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QObject>
#include <QPixmap>
#include <QDateTime>
#include <QHash>

// Forward declarations --------------------
class WebApiCommand;

class Picture : public QObject
{
  Q_OBJECT
public:

  enum PictureRoles {
    RoleId              = Qt::UserRole + 1,
    RoleIdUser          = Qt::UserRole + 2,
    RoleIdSpot          = Qt::UserRole + 3,
    RoleUsername        = Qt::UserRole + 4,
    RoleSpotName        = Qt::UserRole + 5,
    RoleSpotDescription = Qt::UserRole + 6,
    RoleCreated         = Qt::UserRole + 7,
    RoleUrl             = Qt::UserRole + 8
  };

  explicit Picture(QObject *parent = 0);
  explicit Picture(const Picture &other);
  explicit Picture(int id,
                   int idUser,
                   int idSpot, QString username,
                   QString name,
                   QString description, QDateTime created,
                   QObject *parent = 0);
  ~Picture();
  Picture &operator=(const Picture &other);


  int id() const { return m_Id; }

  QString username()        const { return m_Username;        }
  QString spotName()        const { return m_SpotName;        }
  QString spotDescription() const { return m_SpotDescription; }
  QDateTime created() const { return m_Created; }

  QVariant pictureRole(PictureRoles role) const;

  static QHash<int, QByteArray> roleNames();

signals:

  void signal_DownloadThumbFinished();
  void signal_DownloadFinished();

private slots:

  void slot_DownloadThumbFinished(const WebApiError &);
  void slot_DownloadFinished();

private:

  int m_Id;
  int m_IdUser;
  int m_IdSpot;
  QString m_Username;
  QString m_SpotName;
  QString m_SpotDescription;

  QDateTime m_Created;

  QPixmap m_Thumb;
  QPixmap m_Pixmap;

  WebApiCommand *m_WebApiCommand;

};

#endif // PICTURE_H
