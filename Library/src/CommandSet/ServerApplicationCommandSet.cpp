
// Files includes --------------------------
#include "ServerApplicationCommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

void Command_GetServerApplicationVersion::SetResponseParameter_ServerApplicationVersion(const QString &serverApplicationVersion)
{
  Command::setResponseParameter("ServerApplicationVersion",
                                serverApplicationVersion);
}

//-----------------------------------------------------------------------------------------------------------------------------

ServerApplicationCommandSet::ServerApplicationCommandSet()
  : CommandSet()
  , m_Command_GetServerApplicationVersion()
{

}

