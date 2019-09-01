
#ifndef SERVERAPPLICATIONCOMMANDSET_H
#define SERVERAPPLICATIONCOMMANDSET_H

// Project includes ------------------------
#include "../Command/Command.h"
#include "../Command/CommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetServerApplicationVersion : public Command
{

public:

  Command_GetServerApplicationVersion()
    : Command(QString("GetServerApplicationVersion"))
  {;}

  void SetResponseParameter_ServerApplicationVersion(const QString &serverApplicationVersion);
  QString GetResponseParameter_ServerApplicationVersion();

};

//-----------------------------------------------------------------------------------------------------------------------------

class ServerApplicationCommandSet : public CommandSet
{

public:

  ServerApplicationCommandSet();

};

#endif // SERVERAPPLICATIONCOMMANDSET_H
