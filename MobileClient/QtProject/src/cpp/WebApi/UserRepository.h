/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      09.10.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

// Project includes ------------------------
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QObject>
#include <QMap>

// Forward declarations --------------------
class User;

class UserRepository : public QObject
{
  Q_OBJECT

public:

  explicit UserRepository(QObject *parent = 0);
  ~UserRepository();

  static void instanziate();
  static void destroy();

  static UserRepository *instance() { return s_UserRepository; }

  int getNewRequestId();

public slots:

  QList<User *> getUsers(int requestId);

  User *getBy_UserId(int userId);

  User *getAdd_User(int userId,
                    const QString &userUsername);

signals:

  void signal_DataReady(int requestId,
                        bool success);

private slots:

  void slot_Command_Finished(const WebApiError &error);

private:

  static const char* PROPERTY_REQUEST_ID;

  static UserRepository *s_UserRepository;

  int m_RequestId;

  QMap<int, User *> m_QMap_Users;

  QMap<int, QList<User *> > m_QMap_Results;
};


#endif // USERREPOSITORY_H
