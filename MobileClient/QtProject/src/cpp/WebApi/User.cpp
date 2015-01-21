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
#include "../HelperClasses/Logger.h"

// Qt includes -----------------------------
#include <QSettings>

//-----------------------------------------------------------------------------------------------------------------------------

// Public
const QString User::SETTINGS_USERNAME("username");
const QString User::SETTINGS_PASSWORD("password");

// Private
const QString User::C_LOGIN                ("login");
const QString User::C_LOGOUT               ("logout");
const QString User::C_REGISTER             ("register");
const QString User::C_CANREGISTER          ("canregister");
const QString User::R_PARAM_USERNAME       ("username");
const QString User::R_PARAM_PASSWORD       ("password");
const QString User::A_PARAM_AUTHENTICATION ("authentication");

//-----------------------------------------------------------------------------------------------------------------------------

User::User(QSettings *settings,
           QObject *parent) :
  QObject(parent),
  m_Settings           (settings),
  m_LastErrorText      (),
  m_WebApiCommand_Login(this),
  m_WebApiCommand_Logout(this),
  m_WebApiCommand_Register(this),
  m_WebApiCommand_CanRegister(this)
{
  m_WebApiCommand_Login.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Login.setCommand(C_LOGIN);

  m_WebApiCommand_Logout.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Logout.setCommand(C_LOGOUT);

  m_WebApiCommand_Register.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Register.setCommand(C_REGISTER);

  m_WebApiCommand_CanRegister.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_CanRegister.setCommand(C_CANREGISTER);

  connect(&m_WebApiCommand_Login,       SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogin_Finished      (const WebApiError &)));
  connect(&m_WebApiCommand_Logout,      SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogout_Finished     (const WebApiError &)));
  connect(&m_WebApiCommand_Register,    SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandRegister_Finished   (const WebApiError &)));
  connect(&m_WebApiCommand_CanRegister, SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandCanRegister_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

bool User::login()
{
  m_LastErrorText = "";

  QString username       = m_Settings->value(SETTINGS_USERNAME, "").toString();
  QString hashedPassword = m_Settings->value(SETTINGS_PASSWORD, "").toString();

  if(   username.isEmpty()
     || hashedPassword.isEmpty())
  {
    m_LastErrorText = tr("Empty username or password.");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(R_PARAM_PASSWORD, hashedPassword));

  WebApiError error = m_WebApiCommand_Login.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool User::login(const QString &username,
                 const QString &password)
{
  m_LastErrorText = "";

  QString hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512).toHex();

  m_Settings->setValue(SETTINGS_USERNAME, username);
  m_Settings->setValue(SETTINGS_PASSWORD, hashedPassword);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(R_PARAM_PASSWORD, hashedPassword));

  WebApiError error = m_WebApiCommand_Login.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool User::logout()
{
  m_LastErrorText = "";

  m_Settings->setValue(SETTINGS_USERNAME, QString());
  m_Settings->setValue(SETTINGS_PASSWORD, QString());
  m_Settings->sync();

  if(m_WebApiCommand_Login.isRunning())
  {
    Logger::error("User::logout() Command login is running");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  WebApiError error = m_WebApiCommand_Logout.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(error.text());
    return false;
  }

  return true;
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

void User::slot_CommandLogout_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(QString("User::slot_CommandLogout_Finished (%1)").arg(error.text()));
  }

  emit signal_Logout();
  return;
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandRegister_Finished(const WebApiError &error)
{
  // TODO implement me
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandCanRegister_Finished(const WebApiError &error)
{
  // TODO implement me
}

//-----------------------------------------------------------------------------------------------------------------------------


