/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      04.09.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "User.h"

User::User(QObject *parent) :
 QObject(parent),
  m_Id(-1),
  m_Username("")
{

}

User::User(const User &user,
           QObject *parent) :
  QObject(parent),
  m_Id(user.m_Id),
  m_Username(user.m_Username)
{

}

User &User::operator=(const User &other)
{
  m_Id              = other.m_Id;
  m_Username        = other.m_Username;
  return *this;
}

int User::id()
{
  return m_Id;
}

QString User::username()
{
  return m_Username;
}

void User::setId(int id)
{
  m_Id = id;
}

void User::setUsername(const QString &username)
{
  m_Username = username;
}
