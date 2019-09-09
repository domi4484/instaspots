
// Files includes --------------------------
#include "EntityPicture.h"

//-----------------------------------------------------------------------------------------------------------------------------

EntityPicture::EntityPicture()
  : m_Id(-1)
  , m_UserId(-1)
  , m_SpotId(-1)
  , m_QDateTime_Created()
  , m_Latitude(0.0)
  , m_Longitude(0.0)
  , m_Published(false)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_Id(qlonglong id)
{
  m_Id = id;
}

//-----------------------------------------------------------------------------------------------------------------------------

qlonglong EntityPicture::Get_Id() const
{
  return m_Id;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_UserId(qlonglong userId)
{
  m_UserId = userId;
}

//-----------------------------------------------------------------------------------------------------------------------------

qlonglong EntityPicture::Get_UserId() const
{
  return m_UserId;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_SpotId(qlonglong spotId)
{
  m_SpotId = spotId;
}

//-----------------------------------------------------------------------------------------------------------------------------

qlonglong EntityPicture::Get_SpotId() const
{
  return m_SpotId;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_Created(const QDateTime qDateTime_Created)
{
  m_QDateTime_Created = qDateTime_Created;
}

//-----------------------------------------------------------------------------------------------------------------------------

QDateTime EntityPicture::Get_Created() const
{
  return m_QDateTime_Created;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_Latitude(double latitude)
{
  m_Latitude = latitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

double EntityPicture::Get_Latitude() const
{
  return m_Latitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_Longitude(double longitude)
{
  m_Longitude = longitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

double EntityPicture::Get_Longitude() const
{
  return m_Longitude;
}

//-----------------------------------------------------------------------------------------------------------------------------

void EntityPicture::Set_Published(bool published)
{
  m_Published = published;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool EntityPicture::Get_Published() const
{
  return m_Published;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString EntityPicture::Get_Url() const
{
  return QString("https://lowerspot.com/web/pictures/%1/%2.jpg")
                 .arg(m_QDateTime_Created.toString("yyyy/MM/dd"))
                 .arg(m_Id);
}
