#ifndef MAINCOMMANDRECEIVER_H
#define MAINCOMMANDRECEIVER_H

// Library includes ------------------------
#include <Command/CommandReceiver.h>

class MainCommandReceiver : public CommandReceiver
{

public:

  MainCommandReceiver();

protected:

  virtual bool executeCommand(Command *command);

};

#endif // MAINCOMMANDRECEIVER_H
