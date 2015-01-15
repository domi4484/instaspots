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

#ifndef USER_H
#define USER_H

// Project includes ------------------------
#include "WebApiError.h"
#include "WebApiCommand.h"

// Qt includes -----------------------------
#include <QObject>

// Forward declarations --------------------
class QSettings;

class User : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString username READ username)

public:

  static const QString SETTINGS_USERNAME;
  static const QString SETTINGS_PASSWORD;

  explicit User(QSettings *settings,
                QObject *parent = 0);


public slots:
  QString lastErrorText() const { return m_LastErrorText; }

  void login();
  void login(const QString &username,
             const QString &password);

  QString username();

signals:

  void signal_LoginSuccessfull(bool success);

private slots:

  void slot_CommandLogin_Finished(const WebApiError &error);

private:

  static const QString COMMAND;
  static const QString R_PARAM_USERNAME;
  static const QString R_PARAM_PASSWORD;
  static const QString A_PARAM_AUTHENTICATION;

  // Link to Settings
  QSettings *m_Settings;

  QString m_LastErrorText;

  WebApiCommand m_WebApiCommand_Login;
  WebApiCommand m_WebApiCommand_Register;
  WebApiCommand m_WebApiCommand_CanRegister;

};

#endif // USER_H
