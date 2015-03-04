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

// Files includes --------------------------
#include "Picture.h"

// Project includes ------------------------
#include "WebApi.h"
#include "WebApiCommand.h"

Picture::Picture(QObject *parent) :
  QObject(parent),
  m_Id         (-1),
  m_IdUser     (-1),
  m_IdSpot     (-1),
  m_Url            (""),
  m_Username       (""),
  m_SpotName       (""),
  m_SpotDescription(""),
  m_Created        (),
  m_WebApiCommand  (NULL)
{
}

Picture::Picture(const Picture &other) :
  QObject(other.parent()),
  m_Id         (other.m_Id),
  m_IdUser     (other.m_IdUser),
  m_IdSpot     (other.m_IdSpot),
  m_Url            (other.m_Url),
  m_Username       (other.m_Username),
  m_SpotName       (other.m_SpotName),
  m_SpotDescription(other.m_SpotDescription),
  m_Created        (other.m_Created),
  m_WebApiCommand  (NULL)
{

}

Picture::Picture(int id,
                 int idUser,
                 int idSpot,
                 QString url,
                 QString username,
                 QString name,
                 QString description,
                 QDateTime created,
                 QObject *parent) :
  QObject(parent),
  m_Id         (id),
  m_IdUser     (idUser),
  m_IdSpot     (idSpot),
  m_Url            (url),
  m_Username       (username),
  m_SpotName       (name),
  m_SpotDescription(description),
  m_Created        (created),
  m_WebApiCommand  (NULL)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Picture::~Picture()
{
  if(m_WebApiCommand != NULL)
  {
    delete m_WebApiCommand;
    m_WebApiCommand = NULL;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

Picture &Picture::operator=(const Picture &other)
{
  m_Id              = other.m_Id;
  m_IdUser          = other.m_IdUser;
  m_IdSpot          = other.m_IdSpot;
  m_Url             = other.m_Url;
  m_Username        = other.m_Username;
  m_SpotName        = other.m_SpotName;
  m_SpotDescription = other.m_SpotDescription;
  m_Created         = other.m_Created;
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant Picture::pictureRole(PictureRoles role) const
{
  switch (role)
  {
  case RoleId:
    return m_Id;
  break;
  case RoleIdUser:
    return m_IdUser;
  break;
  case RoleIdSpot:
  return m_IdSpot;
  break;
  case RoleUsername:
  return m_Username;
  break;
  case RoleSpotName:
  return m_SpotName;
  break;
  case RoleSpotDescription:
  return m_SpotDescription;
  break;
  case RoleCreated:
  return m_Created;
  break;
  case RoleUrl:
  return m_Url;
  break;
  }
  return "";
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> Picture::roleNames()
{
  QHash<int, QByteArray> roles;
  roles[RoleId]              = "idPicture";
  roles[RoleIdUser]          = "idUser";
  roles[RoleIdSpot]          = "idSpot";
  roles[RoleUsername]        = "username";
  roles[RoleSpotName]        = "spotName";
  roles[RoleSpotDescription] = "spotDescription";
  roles[RoleCreated]         = "created";
  roles[RoleUrl]             = "url";

  return roles;
}


