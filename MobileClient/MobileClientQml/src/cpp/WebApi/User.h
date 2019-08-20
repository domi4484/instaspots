/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      04.09.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef USER_H
#define USER_H

// Qt includes -----------------------------
#include <QObject>

class User : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int     id       READ id      )
  Q_PROPERTY(QString username READ username)

public:

  User(QObject *parent = 0);
  User(const User &user,
       QObject *parent = 0);

  User &operator=(const User &other);

  int id();
  QString username();

  void setId(int id);
  void setUsername(const QString &username);

private:

  int     m_Id;
  QString m_Username;

};

#endif // USER_H
