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

Spot::Spot(QObject *parent) :
  QObject(parent),
  m_Id           (-1),
  m_Name          (""),
  m_Description   (""),
  m_SecretSpot    (false),
  m_Coordinate    (0.0, 0.0),
  m_Distance_km   (-1),
  m_PictureUrl1   (""),
  m_PictureUrl2   (""),
  m_QList_Pictures()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

Spot::~Spot()
{
  m_QList_Pictures.clear();
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
  case RoleSpotSecretSpot:
    return m_SecretSpot;
  break;
  case RoleSpotLatitude:
    return m_Coordinate.latitude();
  break;
  case RoleSpotLongitude:
    return m_Coordinate.longitude();
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
  roles[RoleSpotSecretSpot]  = "role_SpotSecretSpot";
  roles[RoleSpotLatitude]    = "role_SpotLatitude";
  roles[RoleSpotLongitude]   = "role_SpotLongitude";
  roles[RoleSpotDistance]    = "role_SpotDistance";
  roles[RoleSpotPictureUrl1] = "role_SpotPictureUrl1";
  roles[RoleSpotPictureUrl2] = "role_SpotPictureUrl2";

  return roles;
}

//-----------------------------------------------------------------------------------------------------------------------------

int Spot::picturesCount() const
{
  return m_QList_Pictures.size();
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Picture *> Spot::pictures()
{
  return m_QList_Pictures;
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
    if(m_SecretSpot == false)
    {
      return QString("%1 km").arg((int) m_Distance_km);
    }
    else
    {
      return "??? km";
    }
  }
  else
  {
    if(m_SecretSpot == false)
    {
      return QString("%1 m").arg((int) (m_Distance_km*1000));
    }
    else
    {
      return "??? m";
    }
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void Spot::clearPictures()
{
  m_QList_Pictures.clear();

  emit picturesCountChanged();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Spot::addPicture(Picture *picture)
{
  m_QList_Pictures.append(picture);

  emit picturesCountChanged();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Spot::removePicture(Picture *picture)
{
  m_QList_Pictures.removeOne(picture);

  emit picturesCountChanged();
}

//-----------------------------------------------------------------------------------------------------------------------------

