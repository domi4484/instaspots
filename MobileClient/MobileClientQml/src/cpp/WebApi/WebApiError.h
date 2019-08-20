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
#include <QString>

class WebApiError
{

public:

  enum ERROR
  {
    NONE,
    NETWORK,
    SERVER,
    COMMAND,
    COMMAND_ALREADY_RUNNING
  };

  WebApiError(ERROR error,
              QString details = "");

  ERROR type() const { return m_Error; }
  QString text() const;

private:

  ERROR m_Error;
  QString m_Details;

};

#endif // WEBAPIERROR_H
