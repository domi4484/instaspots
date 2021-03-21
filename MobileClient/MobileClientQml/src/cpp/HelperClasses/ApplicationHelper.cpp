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
#include "PlateformDetail.h"
#include "../Settings/Settings.h"
#include "../WebApi/WebApi.h"

// Library includes ------------------------
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QScreen>

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::ApplicationHelper(Settings *settings,
                                     PlateformDetail *plateformDetail,
                                     QObject *parent)
  : QObject(parent),
    m_Settings(settings),
    m_PlateformDetail(plateformDetail),
    m_DipScaleFactor(),
    m_CurrentAvailableClientVersion(),
    m_WebApiCommand_GetCurrentClientVersion(this),
    m_DevelopmentMode(false),
    m_WebApiCommand_ReportProblem(this),
    m_QElapsedTimer_Startup(),
    m_StartupTime_ms(0)
{
  // Log application version
  Logger::info(QString("%1 %2 built on %3 for %4 plateform").arg(QApplication::applicationName())
                                                            .arg(version())
                                                            .arg(buildTimestamp())
                                                            .arg(m_PlateformDetail->name()));

  // Dip scale factor
  double dipNorm;
  switch(m_PlateformDetail->getOS())
  {
    case PlateformDetail::OS_DESKTOP:
      dipNorm = 96.0;
    break;
    case PlateformDetail::OS_ANDROID:
      dipNorm = 160.0;
    break;
    case PlateformDetail::OS_IOS:
      dipNorm = 160.0;
    break;
    case PlateformDetail::OS_WINPHONE:
      dipNorm = 160.0;
    break;
  }
  QScreen *screen = qApp->primaryScreen();
  //double dip = screen->physicalDotsPerInch() / screen->devicePixelRatio() / dipNorm;
  double dip = screen->physicalDotsPerInch() / 108.0;
  if(dip <= 1.0)
    dip = 1.0;
  setDip(dip);

  // Check if newer version was installed
  if(m_Settings->get_Application_LastVersion() != QApplication::applicationVersion())
  {
    m_Settings->set_Application_NewerVersionAvailableGotIt(false);
  }

  m_WebApiCommand_GetCurrentClientVersion.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_GetCurrentClientVersion.setCommandName(WebApi::COMMAND::GET_CURRENT_CLIENT_VERSION);

  connect(&m_WebApiCommand_GetCurrentClientVersion,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandGetCurrentClientVersion_Finished(const WebApiError &)));


  m_WebApiCommand_ReportProblem.setAnswerType(WebApiCommand::JSON);
  m_WebApiCommand_ReportProblem.setCommandName(WebApi::COMMAND::REPORT_PROBLEM);

  connect(&m_WebApiCommand_ReportProblem,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandReportProblem_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::~ApplicationHelper()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

int ApplicationHelper::dip() const
{
  return m_DipScaleFactor;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::setDip(int dipScaleFactor)
{
  m_DipScaleFactor = dipScaleFactor;

  emit signal_Dip_Changed();
}

//-----------------------------------------------------------------------------------------------------------------------------

QString ApplicationHelper::lastErrorText() const
{
  return m_LastErrorText;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::setLastErrorText(const QString &errorText)
{
  m_LastErrorText = errorText;

  emit signal_LastErrorText_Changed();
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

QString ApplicationHelper::currentAvailableClientVersion() const
{
  if(m_CurrentAvailableClientVersion.isEmpty())
  {
    return version();
  }

  return m_CurrentAvailableClientVersion;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool ApplicationHelper::checkCurrentAvailableClientVersion()
{
  if(m_Settings->get_Application_NewerVersionAvailableGotIt() == true)
  {
    return true;
  }

  QList<QueryItem> qList_QueryItems;
  WebApiError error = m_WebApiCommand_GetCurrentClientVersion.postRequest(qList_QueryItems);
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
  m_Settings->set_Application_NewerVersionAvailableGotIt(true);
  m_Settings->sync();
}

//-----------------------------------------------------------------------------------------------------------------------------

QString ApplicationHelper::buildTimestamp() const
{
  QString timestamp = QString("%1 %2").arg(__DATE__).arg(__TIME__);
  timestamp.chop(3);
  return timestamp;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool ApplicationHelper::reportProblem(const QString &problemDescription,
                                      bool attachTraces)
{
  QStringList qStringList_ReportContent;

  qStringList_ReportContent << QString("Version: ------ %1")  .arg(version());
  qStringList_ReportContent << QString("Built on: ------ %1")  .arg(buildTimestamp());
  qStringList_ReportContent << QString("Plateform: ---- %1")  .arg(m_PlateformDetail->name());
  qStringList_ReportContent << QString("Startup time: - %1ms").arg(startupTime_ms());

  if(attachTraces)
  {
    qStringList_ReportContent << QString("Log lines:");
    qStringList_ReportContent << Logger::instance()->getLogEntries().join("\n");
  }


  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::REPORT_TITLE,   problemDescription));
  qList_QueryItems.append(QueryItem(WebApi::PARAMETER::REPORT_CONTENT, qStringList_ReportContent.join("\n")));


  WebApiError error = m_WebApiCommand_ReportProblem.postRequest(qList_QueryItems);
  if(error.type() != WebApiError::NONE)
  {
    setLastErrorText(QString("%1: %2").arg(__FUNCTION__)
                                      .arg(error.text()));
    Logger::error(lastErrorText());
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::startupTimerStart()
{
  m_QElapsedTimer_Startup.start();
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::startupTimerStop()
{
  m_StartupTime_ms = m_QElapsedTimer_Startup.elapsed();
  Logger::info(QString("Application started in %1 ms").arg(m_StartupTime_ms));
}

//-----------------------------------------------------------------------------------------------------------------------------

long ApplicationHelper::startupTime_ms()
{
    return m_StartupTime_ms;
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::slot_CommandGetCurrentClientVersion_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::error(QString("ApplicationHelper::slot_CommandVersionCheck_Finished: %1").arg(error.text()));
    return;
  }

  m_CurrentAvailableClientVersion = m_WebApiCommand_GetCurrentClientVersion.resultParameter(WebApi::PARAMETER::APPLICATION_VERSION).toString();

  if(compareVersions(version(), m_CurrentAvailableClientVersion) >= 0)
  {
    return;
  }

  emit signal_NewClientVersionAvailable();
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::slot_CommandReportProblem_Finished(const WebApiError &error)
{
  if(error.type() != WebApiError::NONE)
  {
    Logger::error(QString("ApplicationHelper::slot_CommandReportProblem_Finished: %1").arg(error.text()));
    return;
  }
}










