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

#ifndef APPLICATIONHELPER_H
#define APPLICATIONHELPER_H

// Project includes ------------------------
#include "../WebApi/WebApiError.h"
#include "../WebApi/WebApiCommand.h"

// Qt includes -----------------------------
#include <QColor>
#include <QObject>
#include <QElapsedTimer>

// Forward declarations --------------------
class Settings;
class PlateformDetail;

class ApplicationHelper : public QObject
{
  Q_OBJECT

  Q_PROPERTY(double  dip           READ dip           WRITE setDip           NOTIFY signal_Dip_Changed          )
  Q_PROPERTY(QString lastErrorText READ lastErrorText WRITE setLastErrorText NOTIFY signal_LastErrorText_Changed)

public:
  explicit ApplicationHelper(Settings *settings,
                             PlateformDetail *plateformDetail,
                             QObject *parent = 0);
  ~ApplicationHelper();

  int dip() const;
  void setDip(int dip);

  QString lastErrorText() const;
  void setLastErrorText(const QString &errorText);

signals:

  void signal_NewClientVersionAvailable();

  void signal_DevelopmentMode_Changed(bool developmentMode);

  void signal_Dip_Changed();
  void signal_LastErrorText_Changed();

public slots:

  // Version
  QString version() const;
  int     compareVersions(const QString &version1,
                          const QString &version2) const;
  QString currentAvailableClientVersion() const;
  bool    checkCurrentAvailableClientVersion();
  void    newerClientVersionAvailableGotIt();

  QString buildTimestamp() const;

  // Problem reporting
  bool reportProblem(const QString &problemDescription,
                     bool attachTraces);

  // Startup timer
  void startupTimerStart();
  void startupTimerStop();
  long startupTime_ms();

  // Colors
  QString color_BackgroundNavigator()      const { return "#7fb5be"; }
  QString color_BackgroundTitleRectangle() const { return "#aaaaaa"; }
  QString color_TextLink()                 const { return "#265f8b"; }
  QString color_TextLink_Pressed()         const { return QColor(color_TextLink()).lighter().name(); }

private slots:

  void slot_CommandGetCurrentClientVersion_Finished(const WebApiError &error);
  void slot_CommandReportProblem_Finished(const WebApiError &error);

private:

  // Link to Settings
  Settings *m_Settings;

  // Link to PlateformDetail
  PlateformDetail *m_PlateformDetail;

  // Dip Scale Factor
  double m_DipScaleFactor;

  // Version
  QString       m_CurrentAvailableClientVersion;
  WebApiCommand m_WebApiCommand_GetCurrentClientVersion;

  // Development mode
  bool m_DevelopmentMode;

  // Report problem
  WebApiCommand m_WebApiCommand_ReportProblem;

  // Startup timer
  QElapsedTimer m_QElapsedTimer_Startup;
  int m_StartupTime_ms;

  // Last error text
  QString m_LastErrorText;
};

#endif // APPLICATIONHELPER_H
