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

#ifndef CURRENTUSER_H
#define CURRENTUSER_H

// Project includes ------------------------
#include "WebApiError.h"
#include "WebApiCommand.h"

// Qt includes -----------------------------
#include <QObject>

// Forward declarations --------------------
class Settings;

class CurrentUser : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString username          READ username NOTIFY signal_Username_changed)
  Q_PROPERTY(int     id                READ id       NOTIFY signal_Id_changed)

public:

  explicit CurrentUser(Settings *settings,
                       QObject *parent = 0);


public slots:

  QString lastErrorText() const { return mLastErrorText; }

  bool isConnected();

  bool login();
  bool login(const QString &username,
             const QString &password);
  bool logout();

  bool registration(const QString &username,
                    const QString &e_mail,
                    const QString &password);

  QString username();
  int id();

signals:

  void signal_LoginSuccessfull(bool success);
  void signal_Logout();
  void signal_RegistrationSuccessfull(bool success);

  void signal_Username_changed();
  void signal_Id_changed();

private slots:

  void slot_CommandLogin_Finished(const WebApiError &error);
  void slot_CommandLogout_Finished(const WebApiError &error);
  void slot_CommandRegister_Finished(const WebApiError &error);
  void slot_CommandCanRegister_Finished(const WebApiError &error);

private:

  // Link to Settings
  Settings *mSettings;

  int mId;
  QString mToken;

  QString mLastErrorText;

  WebApiCommand mWebApiCommand_Login;
  WebApiCommand mWebApiCommand_Logout;
  WebApiCommand mWebApiCommand_Register;
  WebApiCommand mWebApiCommand_CanRegister;
};

#endif // CURRENTUSER_H
