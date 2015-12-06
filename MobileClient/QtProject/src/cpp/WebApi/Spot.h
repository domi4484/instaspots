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

#include <QObject>

class Spot : public QObject
{
    Q_OBJECT
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

    explicit Spot(int     id,
                  QString name,
                  QString description,
                  bool    secretSpot,
                  qreal   latitude,
                  qreal   longitude,
                  qreal   distance_km,
                  QString pictureUrl1,
                  QString pictureUrl2,
                  QObject *parent = 0);
    explicit Spot(QObject *parent = 0);
    ~Spot();

    QVariant spotRole(SpotRoles role) const;
    static QHash<int, QByteArray> roleNames();

    QString distanceText() const;

    void setName(const QString &name)               { m_Name        = name;        }
    void setDescription(const QString &description) { m_Description = description; }
    void setSecretSpot(bool secretSpot)             { m_SecretSpot  = secretSpot;  }
    void setLatitude (qreal latitude)               { m_Latitude    = latitude;    }
    void setLongitude(qreal longitude)              { m_Longitude   = longitude;   }
    void setDistance (qreal distance)               { m_Distance_km = distance;    }
    void setPictureUrl1(const QString &pictureUrl1) { m_PictureUrl1 = pictureUrl1; }
    void setPictureUrl2(const QString &pictureUrl2) { m_PictureUrl2 = pictureUrl2; }

  private:

    int m_Id;
    QString m_Name;
    QString m_Description;
    bool    m_SecretSpot;
    qreal   m_Latitude;
    qreal   m_Longitude;
    qreal   m_Distance_km;
    QString m_PictureUrl1;
    QString m_PictureUrl2;
};

#endif // SPOT_H
