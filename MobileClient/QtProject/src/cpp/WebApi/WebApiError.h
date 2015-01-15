/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      03.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef WEBAPIERROR_H
#define WEBAPIERROR_H

// Qt includes -----------------------------
#include <QObject>

class WebApiError : public QObject
{
  Q_OBJECT

public:

  enum ERROR
  {
    NONE,
    NETWORK,
    SERVER,
    COMMAND,
    COMMAND_ALREADY_RUNNING
  };

  explicit WebApiError(ERROR error,
                       QString details = "",
                       QObject *parent = 0);

  ERROR type() const { return m_Error; }
  QString text() const;

signals:

public slots:

private:

  ERROR m_Error;
  QString m_Details;

};

#endif // WEBAPIERROR_H
