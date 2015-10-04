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
#include "../Settings.h"
#include "../WebApi/WebApi.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QApplication>
#include <QRegularExpression>

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::ApplicationHelper(Settings *settings,
                                     QObject *parent)
  : QObject(parent),
    m_Settings(settings),
    m_CurrentClientVersion(),
    m_WebApiCommand_GetCurrentClientVersion(this),
    m_DevelopmentMode(false)
{
    qDebug() << m_Settings->value(Settings::APPLICATION_LAST_VERSION).toString();
    qDebug() << QApplication::applicationVersion();

    if(m_Settings->value(Settings::APPLICATION_LAST_VERSION).toString() != QApplication::applicationVersion())
    {
        m_Settings->setValue(Settings::APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, false);
    }

    m_WebApiCommand_GetCurrentClientVersion.setAnswerType(WebApiCommand::JSON);
    m_WebApiCommand_GetCurrentClientVersion.setCommand(WebApi::C_GET_CURRENT_CLIENT_VERSION);

    connect(&m_WebApiCommand_GetCurrentClientVersion,
            SIGNAL(signal_Finished(const WebApiError &)),
            SLOT(slot_CommandGetCurrentClientVersion_Finished(const WebApiError &)));
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

int ApplicationHelper::compareVersions(const QString &version1,
                                       const QString &version2) const
{
    QRegularExpression regexp("V(\\d+).(\\d+).(\\d+)");

    // Match version1
    int first1  = 0;
    int second1 = 0;
    int third1  = 0;
    {
        QRegularExpressionMatch match = regexp.match(version1);
        if (match.hasMatch() == false)
        {
            Logger::error(QString("ApplicationHelper::compareVersions: invalid version '%1'").arg(version1));
            return 0;
        }
        first1  = match.captured(1).toInt();
        second1 = match.captured(2).toInt();
        third1  = match.captured(3).toInt();
    }

    // Match version2
    int first2  = 0;
    int second2 = 0;
    int third2  = 0;
    {
        QRegularExpressionMatch match = regexp.match(version2);
        if (match.hasMatch() == false)
        {
            Logger::error(QString("ApplicationHelper::compareVersions: invalid version '%1'").arg(version2));
            return 0;
        }
        first2  = match.captured(1).toInt();
        second2 = match.captured(2).toInt();
        third2  = match.captured(3).toInt();
    }

    if(first1 > first2)
    {
        return 1;
    }
    else if(first1 < first2)
    {
        return -1;
    }
    else
    {
        if(second1 > second2)
        {
            return 1;
        }
        else if(second1 < second2)
        {
            return -1;
        }
        else
        {
            if(third1 > third2)
            {
                return 1;
            }
            else if(third1 < third2)
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

bool ApplicationHelper::checkCurrentClientVersion()
{
    if(m_Settings->value(Settings::APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT).toBool() == true)
    {
        return true;
    }

    WebApiError error = m_WebApiCommand_GetCurrentClientVersion.postRequest();
    if(error.type() != WebApiError::NONE)
    {
      Logger::error(QString("ApplicationHelper::checkForNewerVersion: %1").arg(error.text()));
      return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::newerClientVersionAvailableGotIt()
{
    m_Settings->setValue(Settings::APPLICATION_NEWER_VERSION_AVAILABLE_GOT_IT, true);
    m_Settings->sync();
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

void ApplicationHelper::slot_CommandGetCurrentClientVersion_Finished(const WebApiError &error)
{
    if(error.type() != WebApiError::NONE)
    {
      Logger::error(QString("ApplicationHelper::slot_CommandVersionCheck_Finished: %1").arg(error.text()));
      return;
    }

    m_CurrentClientVersion = m_WebApiCommand_GetCurrentClientVersion.resultParameter(WebApi::A_PARAM_VERSION).toString();

    if(compareVersions(version(), m_CurrentClientVersion) >= 0)
    {
        return;
    }

    emit signal_NewClientVersionAvailable();
}










