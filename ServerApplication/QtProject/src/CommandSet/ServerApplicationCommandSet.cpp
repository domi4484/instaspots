
// Files includes --------------------------
#include "ServerApplicationCommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

Command_GetServerApplicationVersion::Command_GetServerApplicationVersion()
  : Command()
{

}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

ServerApplicationCommandSet::ServerApplicationCommandSet()
  : CommandSet()
  , m_Command_GetServerApplicationVersion()
{

}
