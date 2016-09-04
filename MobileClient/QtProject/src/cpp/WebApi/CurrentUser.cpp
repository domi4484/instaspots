/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "CurrentUser.h"

// Project includes ------------------------
#include "WebApi.h"
#include "../Settings/Settings.h"
#include "../HelperClasses/Logger.h"

//-----------------------------------------------------------------------------------------------------------------------------

CurrenUser::CurrenUser(Settings *settings,
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
  m_WebApiCommand_Login.setCommand(WebApi::COMMAND::LOGIN);

  m_WebApiCommand_Logout.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Logout.setCommand(WebApi::COMMAND::LOGOUT);

  m_WebApiCommand_Register.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_Register.setCommand(WebApi::COMMAND::REGISTER);

  m_WebApiCommand_CanRegister.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_CanRegister.setCommand(WebApi::COMMAND::CANREGISTER);

  connect(&m_WebApiCommand_Login,       SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogin_Finished      (const WebApiError &)));
  connect(&m_WebApiCommand_Logout,      SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogout_Finished     (const WebApiError &)));
  connect(&m_WebApiCommand_Register,    SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandRegister_Finished   (const WebApiError &)));
  connect(&m_WebApiCommand_CanRegister, SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandCanRegister_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrenUser::isConnected()
{
    return m_Id >= 0;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrenUser::login()
{
  m_Id = -1;

  m_LastErrorText = "";

  QString username = m_Settings->get_User_Username();
  QString password = m_Settings->get_User_Password();

  if(   username.isEmpty()
     || password.isEmpty())
  {
    m_LastErrorText = tr("Empty username or password.");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_PASSWORD, password));

  WebApiError error = m_WebApiCommand_Login.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrenUser::login(const QString &username,
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

  m_Settings->set_User_Username(username);
  m_Settings->set_User_Password(password);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_PASSWORD, password));

  WebApiError error = m_WebApiCommand_Login.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrenUser::logout()
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

bool CurrenUser::registration(const QString &username,
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

  m_Settings->set_User_Username(username);
  m_Settings->set_User_Password(password);

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_EMAIL,    e_mail));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_PASSWORD, password));

  WebApiError error = m_WebApiCommand_Register.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString CurrenUser::username()
{
    return m_Settings->get_User_Username();
}

//-----------------------------------------------------------------------------------------------------------------------------

int CurrenUser::id()
{
    return m_Id;
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrenUser::slot_CommandLogin_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    m_LastErrorText = error.text();
    emit signal_LoginSuccessfull(false);
    return;
  }

  if(   m_WebApiCommand_Login.resultParameter(WebApi::PARAMETER::USER_AUTHENTICATED).toBool()
     == false)
  {
    // Reset password in settings
    m_Settings->set_User_LoggedIn(false);
    m_Settings->set_User_Password(QString());
    m_Settings->sync();

    m_Id = -1;
    m_LastErrorText = tr("Authentication failed");
    emit signal_LoginSuccessfull(false);
    emit signal_Username_changed();
    emit signal_Id_changed();
    return;
  }


  m_Id = m_WebApiCommand_Login.resultParameter(WebApi::PARAMETER::USER_USER_ID).toInt();

  m_Settings->set_User_LoggedIn(true);
  m_Settings->sync();
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrenUser::slot_CommandLogout_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(QString("User::slot_CommandLogout_Finished (%1)").arg(error.text()));
  }

  m_Id = -1;
  m_Settings->set_User_LoggedIn(false);
  m_Settings->sync();
  emit signal_Logout();
  return;
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrenUser::slot_CommandRegister_Finished(const WebApiError &error)
{
  if(   error.type() != WebApiError::NONE
     && error.type() != WebApiError::COMMAND)
  {
    m_LastErrorText = error.text();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Registered
  if(   m_WebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_REGISTERED).toBool()
     == false)
  {
    m_Settings->set_User_Password(QString());
    m_Settings->set_User_LoggedIn(false);
    m_Settings->sync();

    m_LastErrorText = m_WebApiCommand_Register.resultParameter(WebApi::CONST::GENERAL_PARAMS::ERROR).toString();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Authenticated
  if(   m_WebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_AUTHENTICATED).toBool()
     == false)
  {
    m_Settings->set_User_Password(QString());
    m_Settings->set_User_LoggedIn(false);
    m_Settings->sync();

    m_Id = -1;
    m_LastErrorText = tr("Authentication failed");
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // User id
  m_Id = m_WebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_USER_ID).toInt();

  m_Settings->set_User_LoggedIn(true);
  m_Settings->sync();
  emit signal_RegistrationSuccessfull(true);
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();

}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrenUser::slot_CommandCanRegister_Finished(const WebApiError &error)
{
  // TODO implement me
}

//-----------------------------------------------------------------------------------------------------------------------------


