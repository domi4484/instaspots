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
#include <QObject>

class ApplicationHelper : public QObject
{
  Q_OBJECT

public:
  explicit ApplicationHelper(QObject *parent = 0);
  ~ApplicationHelper();

signals:

  void signal_DevelopmentMode_Changed(bool developmentMode);

public slots:

  QString version()              const;
  bool    checkForNewerVersion() const;

  bool developmentMode() const;
  void setDevelopmentMode(bool developmentMode);

private slots:

  void slot_CommandVersionCheck_Finished(const WebApiError &);

private:

  WebApiCommand m_WebApiCommand_VersionCheck;

  bool m_DevelopmentMode;
};

#endif // APPLICATIONHELPER_H
