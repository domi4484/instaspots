#ifndef SERVERAPPLICATIONCOMMANDRECEIVER_H
#define SERVERAPPLICATIONCOMMANDRECEIVER_H

// Library includes ------------------------
#include <Command/CommandReceiver.h>
#include <CommandSet/ServerApplicationCommandSet.h>

class ServerApplicationCommandReceiver : public CommandReceiver
{

public:

  ServerApplicationCommandReceiver();

protected:

  virtual void executeCommand(Command *command);

private:

  void executeCommand_GetServerApplicationVersion(Command_GetServerApplicationVersion *command);

};

#endif // SERVERAPPLICATIONCOMMANDRECEIVER_H
