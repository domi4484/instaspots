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

// Library includes ------------------------
#include <HelperClasses/Logger.h>

//-----------------------------------------------------------------------------------------------------------------------------

CurrentUser::CurrentUser(Settings *settings,
                         QObject *parent)
  : QObject(parent)
  , mSettings(settings)
  , mId(-1)
  , mToken()
  , mLastErrorText()
  , mWebApiCommand_Login(this)
  , mWebApiCommand_Logout(this)
  , mWebApiCommand_Register(this)
  , mWebApiCommand_CanRegister(this)
{
  mWebApiCommand_Login.setAnswerType(WebApiCommand::AnswerTypeJSON);
  mWebApiCommand_Login.setRequestType(WebApiCommand::RequestTypePost);
  mWebApiCommand_Login.setCommandName(WebApi::COMMAND::LOGIN);

  mWebApiCommand_Logout.setAnswerType(WebApiCommand::AnswerTypeJSON);
  mWebApiCommand_Logout.setCommandName(WebApi::COMMAND::LOGOUT);

  mWebApiCommand_Register.setAnswerType(WebApiCommand::AnswerTypeJSON);
  mWebApiCommand_Register.setRequestType(WebApiCommand::RequestTypePost);
  mWebApiCommand_Register.setCommandName(WebApi::COMMAND::REGISTER);

  mWebApiCommand_CanRegister.setAnswerType(WebApiCommand::AnswerTypeJSON);
  mWebApiCommand_CanRegister.setCommandName(WebApi::COMMAND::CANREGISTER);

  connect(&mWebApiCommand_Login,       SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogin_Finished      (const WebApiError &)));
  connect(&mWebApiCommand_Logout,      SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandLogout_Finished     (const WebApiError &)));
  connect(&mWebApiCommand_Register,    SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandRegister_Finished   (const WebApiError &)));
  connect(&mWebApiCommand_CanRegister, SIGNAL(signal_Finished(const WebApiError &)), SLOT(slot_CommandCanRegister_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrentUser::isLoggedIn()
{
  return mToken.isEmpty() == false;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrentUser::login()
{
  mToken.clear();

  mLastErrorText = "";

  QString token = mSettings->get_User_Token();

  if(token.isEmpty())
  {
    mLastErrorText = tr("Empty token. New login with username/password needed");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
//  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));

  WebApiError error = mWebApiCommand_Login.sendRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    mLastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrentUser::login(const QString &username,
                        const QString &password)
{
  mId = -1;

  mLastErrorText = "";

  if(username.isEmpty())
  {
      mLastErrorText = tr("Username is empty.");
      return false;
  }

  if(password.isEmpty())
  {
      mLastErrorText = tr("Password is empty.");
      return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_PASSWORD, password));

  WebApiError error = mWebApiCommand_Login.sendRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    mLastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrentUser::logout()
{
  mId = -1;

  mLastErrorText = "";

  emit signal_Username_changed();
  emit signal_Id_changed();

  if(mWebApiCommand_Login.isRunning())
  {
    Logger::error("User::logout() Command login is running");
    return false;
  }

  QList<QueryItem> qList_QueryItems;
  WebApiError error = mWebApiCommand_Logout.sendRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(error.text());
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool CurrentUser::registration(const QString &username,
                               const QString &e_mail,
                               const QString &password)
{
  mLastErrorText = "";

  if(username.isEmpty())
  {
      mLastErrorText = tr("Username is empty.");
      return false;
  }

  if(password.isEmpty())
  {
      mLastErrorText = tr("Password is empty.");
      return false;
  }

  if(e_mail.isEmpty())
  {
     mLastErrorText = tr("E-mail is empty.");
     return false;
  }

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_USERNAME, username));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_EMAIL,    e_mail));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::USER_PASSWORD, password));

  WebApiError error = mWebApiCommand_Register.sendRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    mLastErrorText = error.text();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString CurrentUser::username()
{
  return mSettings->get_User_Username();
}

//-----------------------------------------------------------------------------------------------------------------------------

int CurrentUser::id()
{
    return mId;
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrentUser::slot_CommandLogin_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    // Reset password in settings
    mSettings->set_User_Id(false);
    mSettings->sync();

    mId = -1;
    mToken.clear();
    mLastErrorText = tr("Authentication failed");
    emit signal_LoginSuccessfull(false);
    emit signal_Username_changed();
    emit signal_Id_changed();
    return;
  }

  if(mWebApiCommand_Login.resultParameter(WebApi::PARAMETER::USER_TOKEN).toString().isEmpty())
  {
    // Reset password in settings
    mSettings->set_User_Id(false);
    mSettings->sync();

    mId = -1;
    mToken.clear();
    mLastErrorText = tr("No token");
    emit signal_LoginSuccessfull(false);
    emit signal_Username_changed();
    emit signal_Id_changed();
    return;
  }

  mToken = mWebApiCommand_Login.resultParameter(WebApi::PARAMETER::USER_TOKEN).toString();
  mId = mWebApiCommand_Login.resultParameter(WebApi::PARAMETER::USER_USER_ID).toInt();

  mSettings->set_User_Token(mToken);
  mSettings->set_User_Id(mId);
  mSettings->sync();
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrentUser::slot_CommandLogout_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::warning(QString("User::slot_CommandLogout_Finished (%1)").arg(error.text()));
  }

  mId = -1;
  mToken.clear();
  mSettings->set_User_Id(false);
  mSettings->sync();
  emit signal_Logout();
  return;
}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrentUser::slot_CommandRegister_Finished(const WebApiError &error)
{
  if(   error.type() != WebApiError::NONE
     && error.type() != WebApiError::COMMAND)
  {
    mLastErrorText = error.text();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Registered
  if(   mWebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_REGISTERED).toBool()
     == false)
  {
    mSettings->set_User_Id(false);
    mSettings->sync();

    mLastErrorText = mWebApiCommand_Register.resultParameter(WebApi::CONST::GENERAL_PARAMS::ERROR).toString();
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // Authenticated
  if(   mWebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_TOKEN).toBool()
     == false)
  {
    mSettings->set_User_Id(false);
    mSettings->sync();

    mId = -1;
    mToken.clear();
    mLastErrorText = tr("Authentication failed");
    emit signal_RegistrationSuccessfull(false);
    return;
  }

  // User id
  mId = mWebApiCommand_Register.resultParameter(WebApi::PARAMETER::USER_USER_ID).toInt();

  mSettings->set_User_Id(true);
  mSettings->sync();
  emit signal_RegistrationSuccessfull(true);
  emit signal_LoginSuccessfull(true);
  emit signal_Username_changed();
  emit signal_Id_changed();

}

//-----------------------------------------------------------------------------------------------------------------------------

void CurrentUser::slot_CommandCanRegister_Finished(const WebApiError &error)
{
  Logger::error(QString("CurrentUser::slot_CommandCanRegister_Finished(): %1").arg(error.text()));
  // TODO implement me
}

//-----------------------------------------------------------------------------------------------------------------------------


