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
  m_Id             (-1),
  m_IdUser         (-1),
  m_IdSpot         (-1),
  m_Url            (""),
  m_Username       (""),
  m_SpotName       (""),
  m_SpotDescription(""),
  m_Created        (),
  m_QList_Likers    (),
  m_WebApiCommand  (NULL)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Picture::~Picture()
{
  m_QList_Likers.clear();


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
  m_QList_Likers    = other.m_QList_Likers;
  return *this;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant Picture::pictureRole(PictureRoles role) const
{
  switch (role)
  {
  case Role_PictureId:
    return m_Id;
  break;
  case Role_UserId:
    return m_IdUser;
  break;
  case Role_SpotId:
    return m_IdSpot;
  break;
  case Role_UserUsername:
    return m_Username;
  break;
  case Role_SpotName:
    return m_SpotName;
  break;
  case Role_SpotDescription:
    return m_SpotDescription;
  break;
  case Role_PictureCreated:
    return createdText();
  break;
  case Role_PictureUrl:
    return m_Url;
  break;
  case Role_PictureLikersCount:
    return likersCount();
  break;
  }
  return "";
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> Picture::roleNames()
{
  QHash<int, QByteArray> roles;
  roles[Role_PictureId]          = "role_PictureId";
  roles[Role_PictureCreated]     = "role_PictureCreated";
  roles[Role_PictureUrl]         = "role_PictureUrl";
  roles[Role_UserId]             = "role_UserId";
  roles[Role_UserUsername]       = "role_UserUsername";
  roles[Role_SpotId]             = "role_SpotId";
  roles[Role_SpotName]           = "role_SpotName";
  roles[Role_SpotDescription]    = "role_SpotDescription";
  roles[Role_PictureLikersCount] = "role_PictureLikersCount";

  return roles;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Picture::createdText() const
{
    int elapseTime = m_Created.msecsTo(QDateTime::currentDateTime()) / 1000;

    if(elapseTime < 60)
        return tr("%1s").arg(elapseTime);

    if(elapseTime < 60 * 60)
        return tr("%1m").arg(elapseTime / 60);

    if(elapseTime < 60 * 60 * 24)
        return tr("%1h").arg(elapseTime / (60 * 60));

    if(elapseTime < 60 * 60 * 24 * 7)
        return tr("%1d").arg(elapseTime / (60 * 60 * 24));

    return tr("%1w").arg(elapseTime / (60 * 60 * 24 * 7));
}

//-----------------------------------------------------------------------------------------------------------------------------

int Picture::likersCount() const
{
  return m_QList_Likers.size();
}

//-----------------------------------------------------------------------------------------------------------------------------

QQmlListProperty<User> Picture::likers()
{
  return QQmlListProperty<User>(this,
                                m_QList_Likers);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Picture::clearLikers()
{
  m_QList_Likers.clear();

  emit likersCountChanged();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Picture::addLiker(User *liker)
{
  m_QList_Likers.append(liker);

  emit likersCountChanged();
}





