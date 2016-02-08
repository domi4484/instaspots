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
#include "../Settings/Settings.h"
#include "../HelperClasses/Logger.h"

//-----------------------------------------------------------------------------------------------------------------------------

User::User(Settings *settings,
           QObject *parent) :
  QObject(parent),
  m_Settings                 (settings),
  m_Id                       (-1),
  m_LastErrorText            (),
  m_WebApiCommand_Login      (this),
  m_WebApiCommand_Logout     (this),
  m_WebApiCommand_Register   (this),
  m_WebApiCommand_CanRegister(this)
{
  m_WebApiCommand_Login.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Login.setCommand(WebApi::C_LOGIN);

  m_WebApiCommand_Logout.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Logout.setCommand(WebApi::C_LOGOUT);

  m_WebApiCommand_Register.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Register.setCommand(WebApi::C_REGISTER);

  m_WebApiCommand_CanRegister.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_CanRegister.setCommand(WebApi::C_CANREGISTER);

  connect(&m_WebApiCommand_Login,       SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogin_Finished      (const WebApiError &)));
  connect(&m_WebApiCommand_Logout,      SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogout_Finished     (const WebApiError &)));
  connect(&m_WebApiCommand_Register,    SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandRegister_Finished   (const WebApiError &)));
  connect(&m_WebApiCommand_CanRegister, SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandCanRegister_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

bool User::isConnected()
{
    return m_Id >= 0;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool User::login()
{
  m_Id = -1;

  m_LastErrorText = "";

  QString username       = m_Settings->get_User_Username();
  QString hashedPassword = m_Settings->get_User_Password();

  if(   username.isEmpty()
     || hashedPassword.isEmpty())
  {
    m_LastErrorText = tr("Empty username or password.");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_PASSWORD, hashedPassword));

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
  m_Id = -1;

  m_LastErrorText = "";

  if(username.isEmpty())
  {
      m_LastErrorText = tr("Username is empty.");
      return false;
  }

  if(password.isEmpty())
  {
      m_LastErrorText = tr("Password is empty.");
      return false;
  }

//  QString hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex();

  m_Settings->set_User_Username(username);
  m_Settings->set_User_Password(password);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_PASSWORD, password));

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
  m_Id = -1;

  m_LastErrorText = "";

  // Reset password in settings
  m_Settings->set_User_Password(QString());
  m_Settings->sync();

  emit signal_Username_changed();
  emit signal_Id_changed();

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

bool User::registration(const QString &username,
                        const QString &e_mail,
                        const QString &password)
{
  m_LastErrorText = "";

  if(username.isEmpty())
  {
      m_LastErrorText = tr("Username is empty.");
      return false;
  }

  if(password.isEmpty())
  {
      m_LastErrorText = tr("Password is empty.");
      return false;
  }

  if(e_mail.isEmpty())
  {
     m_LastErrorText = tr("E-mail is empty.");
     return false;
  }

//  QString hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex();

  m_Settings->set_User_Username(username);
  m_Settings->set_User_Password(password);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_EMAIL,    e_mail));
  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_PASSWORD, password));

  WebApiError error = m_WebApiCommand_Register.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString User::username()
{
    return m_Settings->get_User_Username();
}

//-----------------------------------------------------------------------------------------------------------------------------

int User::id()
{
    return m_Id;
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

  if(   m_WebApiCommand_Login.resultParameter(WebApi::A_PARAM_AUTHENTICATION).toBool()
     == false)
  {
    // Reset password in settings
    m_Settings->set_User_Password(QString());
    m_Settings->sync();

    m_Id = -1;
    m_LastErrorText = tr("Authentication failed");
    emit signal_LoginSuccessfull(false);
    emit signal_Username_changed();
    emit signal_Id_changed();
    return;
  }


  m_Id = m_WebApiCommand_Login.resultParameter(WebApi::A_PARAM_ID_USER).toInt();

  m_Settings->sync();
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandLogout_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(QString("User::slot_CommandLogout_Finished (%1)").arg(error.text()));
  }

  m_Id = -1;
  emit signal_Logout();
  return;
}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandRegister_Finished(const WebApiError &error)
{
  if(   error.type() != WebApiError::NONE
     && error.type() != WebApiError::COMMAND)
  {
    m_LastErrorText = error.text();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Registered
  if(   m_WebApiCommand_Register.resultParameter(WebApi::A_PARAM_REGISTERED).toBool()
     == false)
  {
    m_Settings->setValue(Settings::USER_PASSWORD, QString());
    m_Settings->sync();

    m_LastErrorText = m_WebApiCommand_Register.resultParameter(WebApi::CONST::GENERAL_PARAMS::ERROR).toString();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Authenticated
  if(   m_WebApiCommand_Register.resultParameter(WebApi::A_PARAM_AUTHENTICATION).toBool()
     == false)
  {
    m_Settings->setValue(Settings::USER_PASSWORD, QString());
    m_Settings->sync();

    m_Id = -1;
    m_LastErrorText = tr("Authentication failed");
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // User id
  m_Id = m_WebApiCommand_Register.resultParameter(WebApi::A_PARAM_ID_USER).toInt();

  m_Settings->sync();
  emit signal_RegistrationSuccessfull(true);
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();

}

//-----------------------------------------------------------------------------------------------------------------------------

void User::slot_CommandCanRegister_Finished(const WebApiError &error)
{
  // TODO implement me
}

//-----------------------------------------------------------------------------------------------------------------------------


