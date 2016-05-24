/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef SPOT_H
#define SPOT_H

// Qt includes -----------------------------
#include <QObject>
#include <QGeoCoordinate>

class Spot : public QObject
{

    Q_OBJECT

    Q_PROPERTY(int            id           READ id          WRITE setId)
    Q_PROPERTY(QString        name         READ name        WRITE setName        NOTIFY nameChanged       )
    Q_PROPERTY(QString        description  READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool           secretSpot   READ secretSpot  WRITE setSecretSpot  NOTIFY secretSpotChanged )
    Q_PROPERTY(QGeoCoordinate coordinate   READ coordinate  WRITE setCoordinate  NOTIFY coordinateChanged )
    Q_PROPERTY(qreal          distance     READ distance    WRITE setDistance    NOTIFY distanceChanged   )
    Q_PROPERTY(QString        distanceText READ distanceText                     NOTIFY distanceChanged   )
    Q_PROPERTY(QString        pictureUrl1  READ pictureUrl1 WRITE setPictureUrl1 NOTIFY pictureUrl1Changed)
    Q_PROPERTY(QString        pictureUrl2  READ pictureUrl2 WRITE setPictureUrl2 NOTIFY pictureUrl2Changed)
    Q_PROPERTY(int            pictureId1   READ pictureId1  WRITE setPictureId1  NOTIFY pictureId1Changed )
    Q_PROPERTY(int            pictureId2   READ pictureId2  WRITE setPictureId2  NOTIFY pictureId2Changed )

public:

    enum SpotRoles {
      RoleSpotId           = Qt::UserRole + 1,
      RoleSpotName         = Qt::UserRole + 2,
      RoleSpotDescription  = Qt::UserRole + 3,
      RoleSpotSecretSpot   = Qt::UserRole + 4,
      RoleSpotLatitude     = Qt::UserRole + 5,
      RoleSpotLongitude    = Qt::UserRole + 6,
      RoleSpotDistance     = Qt::UserRole + 7,
      RoleSpotPictureUrl1  = Qt::UserRole + 8,
      RoleSpotPictureUrl2  = Qt::UserRole + 9
    };

    explicit Spot(QObject *parent = 0);
    ~Spot();

    QVariant spotRole(SpotRoles role) const;
    static QHash<int, QByteArray> roleNames();

    QString distanceText() const;

    void setId          (int id)                           { m_Id          = id;                   }
    void setName        (const QString &name)              { m_Name        = name;                 emit nameChanged();        }
    void setDescription (const QString &description)       { m_Description = description;          emit descriptionChanged(); }
    void setSecretSpot  (bool secretSpot)                  { m_SecretSpot  = secretSpot;           emit secretSpotChanged();  }
    void setLatitude    (qreal latitude)                   { m_Coordinate.setLatitude(latitude);   emit coordinateChanged();  }
    void setLongitude   (qreal longitude)                  { m_Coordinate.setLongitude(longitude); emit coordinateChanged();  }
    void setCoordinate  (const QGeoCoordinate &coordinate) { m_Coordinate = coordinate;            emit coordinateChanged();  }
    void setDistance    (qreal distance)                   { m_Distance_km = distance;             emit distanceChanged();    }
    void setPictureUrl1 (const QString &pictureUrl1)       { m_PictureUrl1 = pictureUrl1;          emit pictureUrl1Changed(); }
    void setPictureUrl2 (const QString &pictureUrl2)       { m_PictureUrl2 = pictureUrl2;          emit pictureUrl2Changed(); }
    void setPictureId1  (int pictureId)                    { m_PictureId1  = pictureId;            emit pictureId1Changed();  }
    void setPictureId2  (int pictureId)                    { m_PictureId2  = pictureId;            emit pictureId2Changed();  }

    int            id          () const { return m_Id;                     }
    QString        name        () const { return m_Name;                   }
    QString        description () const { return m_Description;            }
    bool           secretSpot  () const { return m_SecretSpot;             }
    qreal          latitude    () const { return m_Coordinate.latitude();  }
    qreal          longitude   () const { return m_Coordinate.longitude(); }
    QGeoCoordinate coordinate  () const { return m_Coordinate;             }
    qreal          distance    () const { return m_Distance_km;            }
    QString        pictureUrl1 () const { return m_PictureUrl1;            }
    QString        pictureUrl2 () const { return m_PictureUrl2;            }
    int            pictureId1  () const { return m_PictureId1;             }
    int            pictureId2  () const { return m_PictureId2;             }

signals:

    void nameChanged       ();
    void descriptionChanged();
    void secretSpotChanged ();
    void coordinateChanged ();
    void distanceChanged   ();
    void pictureUrl1Changed();
    void pictureUrl2Changed();
    void pictureId1Changed ();
    void pictureId2Changed ();

private:

    int m_Id;
    QString        m_Name;
    QString        m_Description;
    bool           m_SecretSpot;
    QGeoCoordinate m_Coordinate;
    qreal          m_Distance_km;
    QString        m_PictureUrl1;
    QString        m_PictureUrl2;
    int            m_PictureId1;
    int            m_PictureId2;
};

#endif // SPOT_H
