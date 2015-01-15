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

// File includes ---------------------------
#include "WebApiError.h"

//-----------------------------------------------------------------------------------------------------------------------------

WebApiError::WebApiError(ERROR error,
                         QString details,
                         QObject *parent) :
  QObject(parent),
  m_Error(error),
  m_Details(details)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

QString WebApiError::text() const
{
  switch (m_Error)
  {
  case NONE:
    return tr("No error");
  break;
  case NETWORK:
    return tr("Network error");
  break;
  case SERVER:
    return tr("Server error");
  break;
  case COMMAND:
    return tr("Command error");
  break;
  case COMMAND_ALREADY_RUNNING:
    return tr("Command already running");
  break;
  }

  return tr("Unknown error");
}

//-----------------------------------------------------------------------------------------------------------------------------

