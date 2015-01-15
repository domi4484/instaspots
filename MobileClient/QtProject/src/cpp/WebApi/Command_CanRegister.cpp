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
#include "Command_CanRegister.h"

//-----------------------------------------------------------------------------------------------------------------------------

const QString Command_CanRegister::COMMAND             ("canregister");
const QString Command_CanRegister::R_PARAM_USERNAME    ("username");
const QString Command_CanRegister::A_PARAM_CANREGISTER ("canregister");

//-----------------------------------------------------------------------------------------------------------------------------

Command_CanRegister::Command_CanRegister(QObject *parent) :
    WebApiCommand(parent),
    m_Running(false),
    m_Can(false)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_CanRegister::execute(const QString &username)
{
    if (m_Running == true)
        return;
    m_Running = true;
    m_Can = false;

    QList<QueryItem> qList_QueryItems;
    qList_QueryItems.append(QueryItem(R_PARAM_USERNAME, username));

    postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Command_CanRegister::can()
{
  return m_Can;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command_CanRegister::command()
{
    return COMMAND;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_CanRegister::setResult(const WebApiError &error,
                                    const QScriptValue &result)
{
  m_Running = false;

  if(error.type() != WebApiError::NONE)
  {
    emit signal_Finished(error);
    return;
  }

  m_Can = result.property(A_PARAM_CANREGISTER).toBool();
  emit signal_Finished(error);
}

//-----------------------------------------------------------------------------------------------------------------------------
