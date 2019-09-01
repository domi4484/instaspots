#ifndef SPOTCOMMANDRECEIVER_H
#define SPOTCOMMANDRECEIVER_H

// Library includes ------------------------
#include <Command/CommandReceiver.h>
#include <CommandSet/SpotCommandSet.h>

class SpotCommandReceiver : public CommandReceiver
{

public:

  SpotCommandReceiver();

protected:

  virtual void executeCommand(Command *command);

private:

  void executeCommand_GetServerApplicationVersion(Command_GetSpot *command);

};

#endif // SPOTCOMMANDRECEIVER_H
