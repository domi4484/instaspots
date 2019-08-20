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

#include <QApplication>

//-----------------------------------------------------------------------------------------------------------------------------

WebApiError::WebApiError(ERROR error,
                         QString details) :
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
    return "No error";
  break;
  case NETWORK:
    return "Network error";
  break;
  case SERVER:
    return "Server error";
  break;
  case COMMAND:
    return "Command error";
  break;
  case COMMAND_ALREADY_RUNNING:
    return "Command already running";
  break;
  }

  return "Unknown error";
}

//-----------------------------------------------------------------------------------------------------------------------------

