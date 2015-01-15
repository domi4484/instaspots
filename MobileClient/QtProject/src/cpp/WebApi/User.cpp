/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "User.h"

// Project includes ------------------------
#include "WebApi.h"

// Qt includes -----------------------------
#include <QSettings>

//-----------------------------------------------------------------------------------------------------------------------------

// Public
const QString User::SETTINGS_USERNAME("username");
const QString User::SETTINGS_PASSWORD("password");

// Private
const QString User::COMMAND                ("login");
const QString User::R_PARAM_USERNAME       ("username");
const QString User::R_PARAM_PASSWORD       ("password");
const QString User::A_PARAM_AUTHENTICATION ("authentication");

//-----------------------------------------------------------------------------------------------------------------------------

User::User(QSettings *settings,
           QObject *parent) :
  QObject(parent),
  m_Settings           (settings),
  m_LastErrorText      (),
  m_WebApiCommand_Login(this)
{
  m_WebApiCommand_Login.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Login.setCommand(COMMAND);
  connect(&m_WebApiCommand_Login,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandLogin_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::login()
{
  QString username       = m_Settings->value(SETTINGS_USERNAME, "").toString();
  QString hashedPassword = m_Settings->value(SETTINGS_PASSWORD, "").toString();

  if(   username.isEmpty()
     || hashedPassword.isEmpty())
  {
    emit signal_LoginSuccessfull(false);
    return;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(R_PARAM_PASSWORD, hashedPassword));

  m_WebApiCommand_Login.postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::login(const QString &username,
                 const QString &password)
{
  QString hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512).toHex();

  m_Settings->setValue(SETTINGS_USERNAME, username);
  m_Settings->setValue(SETTINGS_PASSWORD, hashedPassword);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(R_PARAM_PASSWORD, hashedPassword));

  m_WebApiCommand_Login.postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString User::username()
{
  return m_Settings->value(SETTINGS_USERNAME, "").toString();
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandLogin_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    emit signal_LoginSuccessfull(false);
    return;
  }

  if(   m_WebApiCommand_Login.resultParameter(A_PARAM_AUTHENTICATION).toBool()
     == false)
  {
    m_Settings->setValue(SETTINGS_USERNAME, QString());
    m_Settings->setValue(SETTINGS_PASSWORD, QString());
    m_Settings->sync();

    m_LastErrorText = tr("Authentication failed");
    emit signal_LoginSuccessfull(false);
    return;
  }

  m_Settings->sync();
  emit signal_LoginSuccessfull(true);
}

//-----------------------------------------------------------------------------------------------------------------------------


