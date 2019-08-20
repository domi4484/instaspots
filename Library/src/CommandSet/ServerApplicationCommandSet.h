
#ifndef SERVERAPPLICATIONCOMMANDSET_H
#define SERVERAPPLICATIONCOMMANDSET_H

#include "../Command/Command.h"
#include "../Command/CommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetServerApplicationVersion : public Command
{

public:

  Command_GetServerApplicationVersion()
    : Command()
  {;}

};

//-----------------------------------------------------------------------------------------------------------------------------

class ServerApplicationCommandSet : public CommandSet
{

public:

  ServerApplicationCommandSet();

private:

  Command_GetServerApplicationVersion m_Command_GetServerApplicationVersion;
};

#endif // SERVERAPPLICATIONCOMMANDSET_H
