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

TODO
//  Q_PROPERTY(int            id           READ id          WRITE setId)
//  Q_PROPERTY(QString        username     READ username    WRITE setUsername        NOTIFY nameChanged       )
//  Q_PROPERTY(QString        description  READ description WRITE setDescription NOTIFY descriptionChanged)
//  Q_PROPERTY(bool           secretSpot   READ secretSpot  WRITE setSecretSpot  NOTIFY secretSpotChanged )
//  Q_PROPERTY(QGeoCoordinate coordinate   READ coordinate  WRITE setCoordinate  NOTIFY coordinateChanged )
//  Q_PROPERTY(qreal          distance     READ distance    WRITE setDistance    NOTIFY distanceChanged   )
//  Q_PROPERTY(QString        distanceText READ distanceText                     NOTIFY distanceChanged   )
//  Q_PROPERTY(QString        pictureUrl1  READ pictureUrl1 WRITE setPictureUrl1 NOTIFY pictureUrl1Changed)
//  Q_PROPERTY(QString        pictureUrl2  READ pictureUrl2 WRITE setPictureUrl2 NOTIFY pictureUrl2Changed)
//  Q_PROPERTY(int            pictureId1   READ pictureId1  WRITE setPictureId1  NOTIFY pictureId1Changed )
//  Q_PROPERTY(int            pictureId2   READ pictureId2  WRITE setPictureId2  NOTIFY pictureId2Changed )

public:

  enum PictureRoles {
    Role_PictureId       = Qt::UserRole + 1,
    Role_UserId          = Qt::UserRole + 2,
    Role_SpotId          = Qt::UserRole + 3,
    Role_UserUsername    = Qt::UserRole + 4,
    Role_SpotName        = Qt::UserRole + 5,
    Role_SpotDescription = Qt::UserRole + 6,
    Role_PictureCreated  = Qt::UserRole + 7,
    Role_PictureUrl      = Qt::UserRole + 8,
    Role_PictureLikersCount     = Qt::UserRole + 9
  };

  explicit Picture(QObject *parent = 0);
  ~Picture();
  Picture &operator=(const Picture &other);

  QVariant pictureRole(PictureRoles role) const;
  static QHash<int, QByteArray> roleNames();

  int         id()              const { return m_Id; }
  QString     username()        const { return m_Username;        }
  QString     spotName()        const { return m_SpotName;        }
  QString     spotDescription() const { return m_SpotDescription; }
  QDateTime   created()         const { return m_Created; }
  QString     createdText()     const;
  int         likersCount()     const;
  QStringList likersUsername()  const;

  void setId              (int id)                         { m_Id              = id;              }
  void setIdUser          (int idUser)                     { m_IdUser          = idUser;          }
  void setIdSpot          (int idSpot)                     { m_IdSpot          = idSpot;          }
  void setUrl             (const QString &url)             { m_Url             = url;             }
  void setUsername        (const QString &username)        { m_Username        = username;        }
  void setSpotName        (const QString &spotName)        { m_SpotName        = spotName;        }
  void setSpotDescription (const QString &spotDescription) { m_SpotDescription = spotDescription; }
  void setCreated         (const QDateTime &created)       { m_Created         = created;         }

  void clearLikers();
  void addLiker(int idUser,
                const QString &username);

signals:

  void signal_DownloadThumbFinished();
  void signal_DownloadFinished();

private:

  int                m_Id;
  int                m_IdUser;
  int                m_IdSpot;
  QString            m_Url;
  QString            m_Username;
  QString            m_SpotName;
  QString            m_SpotDescription;
  QDateTime          m_Created;
  QMap<int, QString> m_QMap_Likers;

  WebApiCommand *m_WebApiCommand;

};

#endif // PICTURE_H
