
// Files includes --------------------------
#include "ServerApplicationCommandReceiver.h"

// Qt includes -----------------------------
#include <QCoreApplication>

//-----------------------------------------------------------------------------------------------------------------------------

ServerApplicationCommandReceiver::ServerApplicationCommandReceiver()
  : CommandReceiver()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ServerApplicationCommandReceiver::executeCommand(Command *command)
{
  if(command->GetName() == Command_GetServerApplicationVersion().GetName())
    executeCommand_GetServerApplicationVersion((Command_GetServerApplicationVersion *)command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void ServerApplicationCommandReceiver::executeCommand_GetServerApplicationVersion(Command_GetServerApplicationVersion *command)
{
  command->SetResponseParameter_ServerApplicationVersion(QCoreApplication::applicationVersion());
}
