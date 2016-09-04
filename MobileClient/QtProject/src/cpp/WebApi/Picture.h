/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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
#include "User.h"

// Qt includes -----------------------------
#include <QObject>
#include <QPixmap>
#include <QDateTime>
#include <QHash>
#include <QQmlListProperty>

// Forward declarations --------------------
class WebApiCommand;

class Picture : public QObject
{

  Q_OBJECT

  Q_PROPERTY(int            id              READ id              WRITE setId)
  Q_PROPERTY(QString        username        READ username        WRITE setUsername        NOTIFY usernameChanged        )
  Q_PROPERTY(QString        spotName        READ spotName        WRITE setSpotName        NOTIFY spotNameChanged        )
  Q_PROPERTY(QString        spotDescription READ spotDescription WRITE setSpotDescription NOTIFY spotDescriptionChanged )
  Q_PROPERTY(QDateTime      created         READ created         WRITE setCreated         NOTIFY createdChanged         )
  Q_PROPERTY(QString        createdText     READ createdText                              NOTIFY createdChanged         )
  Q_PROPERTY(int            likersCount     READ likersCount                              NOTIFY likersCountChanged     )
  Q_PROPERTY(QQmlListProperty<User>    likersUsername  READ likersUsername                           NOTIFY likersUsernameChanged  )

public:

  enum PictureRoles {
    Role_PictureId          = Qt::UserRole + 1,
    Role_UserId             = Qt::UserRole + 2,
    Role_SpotId             = Qt::UserRole + 3,
    Role_UserUsername       = Qt::UserRole + 4,
    Role_SpotName           = Qt::UserRole + 5,
    Role_SpotDescription    = Qt::UserRole + 6,
    Role_PictureCreated     = Qt::UserRole + 7,
    Role_PictureUrl         = Qt::UserRole + 8,
    Role_PictureLikersCount = Qt::UserRole + 9
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
  QQmlListProperty<User> likersUsername();

  void setId              (int id)                         { m_Id              = id;       }
  void setIdUser          (int idUser)                     { m_IdUser          = idUser;   }
  void setIdSpot          (int idSpot)                     { m_IdSpot          = idSpot;   }
  void setUrl             (const QString &url)             { m_Url             = url;      }
  void setUsername        (const QString &username)        { m_Username        = username;        emit usernameChanged();        }
  void setSpotName        (const QString &spotName)        { m_SpotName        = spotName;        emit spotNameChanged();        }
  void setSpotDescription (const QString &spotDescription) { m_SpotDescription = spotDescription; emit spotDescriptionChanged(); }
  void setCreated         (const QDateTime &created)       { m_Created         = created;         emit createdChanged();         }

  void clearLikers();
  void addLiker(int idUser,
                const QString &username);

signals:

  void usernameChanged();
  void spotNameChanged();
  void spotDescriptionChanged();
  void createdChanged();
  void likersCountChanged();
  void likersUsernameChanged();

private:

  int                m_Id;
  int                m_IdUser;
  int                m_IdSpot;
  QString            m_Url;
  QString            m_Username;
  QString            m_SpotName;
  QString            m_SpotDescription;
  QDateTime          m_Created;
  QList<User *>    m_QList_Likers;

  WebApiCommand *m_WebApiCommand;

};

#endif // PICTURE_H
