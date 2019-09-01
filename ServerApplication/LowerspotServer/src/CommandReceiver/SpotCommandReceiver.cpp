
// Files includes --------------------------
#include "SpotCommandReceiver.h"

// Qt includes -----------------------------
#include <QCoreApplication>

//-----------------------------------------------------------------------------------------------------------------------------

SpotCommandReceiver::SpotCommandReceiver()
  : CommandReceiver()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotCommandReceiver::executeCommand(Command *command)
{
  if(command->GetName() == Command_GetSpot().GetName())
    executeCommand_GetServerApplicationVersion((Command_GetSpot *)command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void SpotCommandReceiver::executeCommand_GetServerApplicationVersion(Command_GetSpot *command)
{

}
