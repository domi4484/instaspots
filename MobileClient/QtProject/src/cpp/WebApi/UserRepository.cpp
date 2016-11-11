/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      09.10.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "UserRepository.h"

// Project includes ------------------------
#include "User.h"
#include "WebApi.h"
#include "WebApiCommand.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const char* UserRepository::PROPERTY_REQUEST_ID("request_id");

//-----------------------------------------------------------------------------------------------------------------------------

UserRepository *UserRepository::s_UserRepository = NULL;

//-----------------------------------------------------------------------------------------------------------------------------

UserRepository::UserRepository(QObject *parent) :
  QObject(parent),
  m_RequestId(1),
  m_QMap_Users(),
  m_QMap_Results()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

UserRepository::~UserRepository()
{
  foreach (int userId, m_QMap_Users.keys())
  {
    delete m_QMap_Users.take(userId);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void UserRepository::instanziate()
{
    if(s_UserRepository != NULL)
    {
      qWarning() << "UserRepository is already initialised.";
      return;
    }

    s_UserRepository = new UserRepository();

}

//-----------------------------------------------------------------------------------------------------------------------------

void UserRepository::destroy()
{
  if(s_UserRepository == NULL)
  {
    qWarning() << "UserRepository instance is already null.";
    return;
  }

  delete s_UserRepository;
  s_UserRepository = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------

int UserRepository::getNewRequestId()
{
    return ++m_RequestId;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<User *> UserRepository::getUsers(int requestId)
{
  return m_QMap_Results.value(requestId);
}

//-----------------------------------------------------------------------------------------------------------------------------

User *UserRepository::getBy_UserId(int userId)
{
  if(userId < 0)
    return NULL;

  User *user = m_QMap_Users.value(userId, NULL);
  if(user == NULL)
  {
    user = new User(this);
    user->setId(userId);
    m_QMap_Users.insert(userId, user);

    // #todo Get internet
  }

  return user;
}

//-----------------------------------------------------------------------------------------------------------------------------

User *UserRepository::getAdd_User(int userId,
                                  const QString &userUsername)
{
  if(userId < 0)
    return NULL;

  User *user = m_QMap_Users.value(userId, NULL);
  if(user == NULL)
  {
    user = new User(this);
    user->setId(userId);
    m_QMap_Users.insert(userId, user);
  }
  user->setUsername(userUsername);

  return user;
}

//-----------------------------------------------------------------------------------------------------------------------------

void UserRepository::slot_Command_Finished(const WebApiError &error)
{
  QList<User *> newUsers;
  int requestId = sender()->property(PROPERTY_REQUEST_ID).toInt();

  if(   error.type() != WebApiError::NONE
     && requestId != 0)
  {
    emit signal_DataReady(requestId,
                          false);
    m_QMap_Results.insert(requestId,
                          QList<User *>());
    return;
  }

  WebApiCommand *webApiCommand = dynamic_cast<WebApiCommand *>(sender());
  QJsonArray jsonArray_Users = webApiCommand->resultArray(WebApi::PARAMETER::USER_LIST);
  for(int i = 0; i < jsonArray_Users.size(); i++)
  {
    QJsonObject jsonObject_User = jsonArray_Users.at(i).toObject();

    int user_id = jsonObject_User.value(WebApi::PARAMETER::USER_USER_ID).toInt();

    User *user = m_QMap_Users.value(user_id, NULL);
    if(user == NULL)
    {
      user = new User(this);
      user->setId(user_id);
      m_QMap_Users.insert(user_id, user);
    }

    user->setUsername(jsonObject_User.value(WebApi::PARAMETER::USER_USERNAME).toString());

    // Add to current request
    newUsers.append(user);
  }

  webApiCommand->deleteLater();

  if(requestId != 0)
  {
    m_QMap_Results.insert(requestId,
                          newUsers);
    emit signal_DataReady(requestId,
                          true);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

