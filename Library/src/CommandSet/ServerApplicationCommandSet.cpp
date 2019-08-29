
// Files includes --------------------------
#include "ServerApplicationCommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

void Command_GetServerApplicationVersion::SetResponseParameter_ServerApplicationVersion(const QString &serverApplicationVersion)
{
  Command::setResponseParameter("ServerApplicationVersion",
                                serverApplicationVersion);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Command_GetServerApplicationVersion::GetResponseParameter_ServerApplicationVersion()
{
  QString serverApplicationVersion;
  Command::getResponseParameter("ServerApplicationVersion",
                                serverApplicationVersion);
  return serverApplicationVersion;
}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

ServerApplicationCommandSet::ServerApplicationCommandSet()
  : CommandSet()
  , m_Command_GetServerApplicationVersion()
{

}

