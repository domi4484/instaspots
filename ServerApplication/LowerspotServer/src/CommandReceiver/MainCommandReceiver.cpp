
// Files includes --------------------------
#include "MainCommandReceiver.h"

//-----------------------------------------------------------------------------------------------------------------------------

MainCommandReceiver::MainCommandReceiver()
  : CommandReceiver()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

bool MainCommandReceiver::executeCommand(Command *command)
{
  bool commandExecuted = false;
  foreach (CommandReceiver *commandReceiver, m_QList_ChildCommandReceiver)
  {
    if(commandReceiver->executeCommand(command) == true)
    {
      commandExecuted = true;
      break;
    }
  }

  if(commandExecuted == false)
    throw exception
}

//-----------------------------------------------------------------------------------------------------------------------------
