/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "Command_Register.h"

//-----------------------------------------------------------------------------------------------------------------------------

const QString Command_Register::COMMAND            ("register");

const QString Command_Register::R_PARAM_USERNAME   ("username");
const QString Command_Register::R_PARAM_PASSWORD   ("password");
const QString Command_Register::R_PARAM_EMAIL      ("email");

const QString Command_Register::A_PARAM_REGISTERED ("registered");

//-----------------------------------------------------------------------------------------------------------------------------

Command_Register::Command_Register(QObject *parent) :
    WebApiCommand(parent),
    m_Running(false),
    m_Success(false)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_Register::execute(const QString &username,
                               const QString &password,
                               const QString &eMail)
{
  if(m_Running)
    return;

  m_Running = true;
  m_Success = false;

  QList<QueryItem> qList_QueryItems;
  qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));
  qList_QueryItems.append(QueryItem(R_PARAM_PASSWORD, password));
  qList_QueryItems.append(QueryItem(R_PARAM_EMAIL,    eMail));

  postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Command_Register::success()
{
  return m_Success;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command_Register::command()
{
    return COMMAND;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_Register::setResult(const WebApiError &error,
                                 const QScriptValue &result)
{
  m_Running = false;

  if(error.type() != WebApiError::NONE)
  {
    emit signal_Finished(error);
    return;
  }

  m_Success = result.property(A_PARAM_REGISTERED).toBool();
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------
