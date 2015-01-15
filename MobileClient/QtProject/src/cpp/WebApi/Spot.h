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
      RoleSpotLatitude     = Qt::UserRole + 4,
      RoleSpotLongitude    = Qt::UserRole + 5,
      RoleSpotDistance     = Qt::UserRole + 6,
      RoleSpotPictureUrl1  = Qt::UserRole + 7,
      RoleSpotPictureUrl2  = Qt::UserRole + 8
    };

    explicit Spot(int     id,
                  QString name,
                  QString description,
                  qreal   latitude,
                  qreal   longitude,
                  qreal   distance,
                  QString pictureUrl1,
                  QString pictureUrl2,
                  QObject *parent = 0);
    ~Spot();


    QVariant spotRole(SpotRoles role) const;

    static QHash<int, QByteArray> roleNames();

  signals:

  public slots:

  private:

    int m_Id;
    QString m_Name;
    QString m_Description;
    qreal   m_Latitude;
    qreal   m_Longitude;
    QString m_Distance;
    QString m_PictureUrl1;
    QString m_PictureUrl2;
};

#endif // SPOT_H
