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

// Files includes --------------------------
#include "Spot.h"

// Project includes ------------------------
#include "WebApi.h"

// Qt includes -----------------------------
#include <QVariant>

//-----------------------------------------------------------------------------------------------------------------------------

Spot::Spot(int     id,
           QString name,
           QString description,
           qreal   latitude,
           qreal   longitude,
           qreal   distance_km,
           QString pictureUrl1,
           QString pictureUrl2,
           QObject *parent)
  : QObject(parent),
    m_Id          (id),
    m_Name        (name),
    m_Description (description),
    m_Latitude    (latitude),
    m_Longitude   (longitude),
    m_Distance_km (distance_km),
    m_PictureUrl1 (pictureUrl1),
    m_PictureUrl2 (pictureUrl2)
{
  if(m_PictureUrl1.isEmpty() == false)
  {
    m_PictureUrl1 = m_PictureUrl1;
  }

  if(m_PictureUrl2.isEmpty() == false)
  {
    m_PictureUrl2 = m_PictureUrl2;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

Spot::~Spot()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant Spot::spotRole(Spot::SpotRoles role) const
{
  switch (role)
  {
  case RoleSpotId:
    return m_Id;
  break;
  case RoleSpotName:
    return m_Name;
  break;
  case RoleSpotDescription:
    return m_Description;
  break;
  case RoleSpotLatitude:
    return m_Latitude;
  break;
  case RoleSpotLongitude:
    return m_Longitude;
  break;
  case RoleSpotDistance:
    return distanceText();
  break;
  case RoleSpotPictureUrl1:
    return m_PictureUrl1;
  break;
  case RoleSpotPictureUrl2:
    return m_PictureUrl2;
  break;
  }
  return "";
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> Spot::roleNames()
{
  QHash<int, QByteArray> roles;
  roles[RoleSpotId]          = "role_SpotId";
  roles[RoleSpotName]        = "role_SpotName";
  roles[RoleSpotDescription] = "role_SpotDescription";
  roles[RoleSpotLatitude]    = "role_SpotLatitude";
  roles[RoleSpotLongitude]   = "role_SpotLongitude";
  roles[RoleSpotDistance]    = "role_SpotDistance";
  roles[RoleSpotPictureUrl1] = "role_SpotPictureUrl1";
  roles[RoleSpotPictureUrl2] = "role_SpotPictureUrl2";

  return roles;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Spot::distanceText() const
{
  if(m_Distance_km < 0)
  {
      return "";
  }

  if(m_Distance_km >= 2)
  {
    return QString("%1 km").arg((int) m_Distance_km);
  }
  else
  {
    return QString("%1 m").arg((int) (m_Distance_km*1000));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

