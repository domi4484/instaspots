/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      07.04.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "ApplicationHelper.h"

// Project includes ------------------------
#include "Logger.h"
#include "../WebApi/WebApi.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QApplication>

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::ApplicationHelper(QObject *parent)
  : QObject(parent),
    m_WebApiCommand_VersionCheck(this),
    m_DevelopmentMode(false)
{
    m_WebApiCommand_VersionCheck.setAnswerType(WebApiCommand::JSON);
    m_WebApiCommand_VersionCheck.setCommand(WebApi::C_GET_CURRENT_CLIENT_VERSION);

    connect(&m_WebApiCommand_VersionCheck,
            SIGNAL(signal_Finished(const WebApiError &)),
            SLOT(slot_CommandVersionCheck_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::~ApplicationHelper()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QString ApplicationHelper::version() const
{
    return QApplication::applicationVersion();
}

//-----------------------------------------------------------------------------------------------------------------------------

bool ApplicationHelper::checkForNewerVersion() const
{
    m_Id = -1;

    m_LastErrorText = "";

    QString username       = m_Settings->value(Settings::USER_USERNAME, "").toString();
    QString hashedPassword = m_Settings->value(Settings::USER_PASSWORD, "").toString();

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

bool ApplicationHelper::developmentMode() const
{
    return m_DevelopmentMode;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::setDevelopmentMode(bool developmentMode)
{
  m_DevelopmentMode = developmentMode;

  if(developmentMode)
  {
    Logger::debug(tr("WebApi development url: %1").arg(WebApi::URL_DEVELOPMENT));
    WebApi::instance()->setUrl(WebApi::URL_DEVELOPMENT);
  }
  else
  {
    Logger::debug(tr("WebApi production url: %1").arg(WebApi::URL_PRODUCTION));
    WebApi::instance()->setUrl(WebApi::URL_PRODUCTION);
  }

  emit signal_DevelopmentMode_Changed(developmentMode);
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::slot_CommandVersionCheck_Finished(const WebApiError &)
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
      m_Settings->setValue(Settings::USER_USERNAME, QString());
      m_Settings->setValue(Settings::USER_PASSWORD, QString());
      m_Settings->sync();

      m_Id = -1;
      m_LastErrorText = tr("Authentication failed");
      emit signal_LoginSuccessfull(false);
      return;
    }


    m_Id = m_WebApiCommand_Login.resultParameter(WebApi::A_PARAM_ID_USER).toInt();

    m_Settings->sync();
    emit signal_LoginSuccessfull(true);
}










