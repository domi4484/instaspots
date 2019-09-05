
// Files includes --------------------------
#include "ApplicationCommandReceiver.h"
#include "Entity/EntityPicture.h"
#include "Database/DatabaseManager.h"
#include "Database/DatabaseTablePicture.h"

// Qt includes -----------------------------
#include <QCoreApplication>

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationCommandReceiver::ApplicationCommandReceiver(DatabaseManager *databaseManager)
  : CommandReceiver()
  , m_DatabaseManager(databaseManager)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand(Command *command)
{
  if(command->GetName() == Command_GetSpot().GetName())
    executeCommand_GetSpot((Command_GetSpot *)command);
  if(command->GetName() == Command_GetSpotsByDistance().GetName())
    executeCommand_GetSpotsByDistance((Command_GetSpotsByDistance *)command);
  if(command->GetName() == Command_GetSpotsByUser().GetName())
    executeCommand_GetSpotsByUser((Command_GetSpotsByUser *)command);
  if(command->GetName() == Command_GetPicturesByNewest().GetName())
    executeCommand_GetPicturesByNewest((Command_GetPicturesByNewest *)command);
  if(command->GetName() == Command_GetPicturesBySpot().GetName())
    executeCommand_GetPicturesBySpot((Command_GetPicturesBySpot *)command);
  if(command->GetName() == Command_GetPicturesByUser().GetName())
    executeCommand_GetPicturesByUser((Command_GetPicturesByUser *)command);
  if(command->GetName() == Command_UploadPictureToSpot().GetName())
    executeCommand_UploadPictureToSpot((Command_UploadPictureToSpot *)command);
  if(command->GetName() == Command_UploadNewSpot().GetName())
    executeCommand_UploadNewSpot((Command_UploadNewSpot *)command);
  if(command->GetName() == Command_PictureRemove().GetName())
    executeCommand_PictureRemove((Command_PictureRemove *)command);
  if(command->GetName() == Command_SignalDisappearedSpot().GetName())
    executeCommand_SignalDisappearedSpot((Command_SignalDisappearedSpot *)command);
  if(command->GetName() == Command_PictureLike().GetName())
    executeCommand_PictureLike((Command_PictureLike *)command);
  if(command->GetName() == Command_PictureUnlike().GetName())
    executeCommand_PictureUnlike((Command_PictureUnlike *)command);

  if(command->GetName() == Command_GetServerApplicationVersion().GetName())
    executeCommand_GetServerApplicationVersion((Command_GetServerApplicationVersion *)command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetSpot(Command_GetSpot *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetSpotsByDistance(Command_GetSpotsByDistance *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetSpotsByUser(Command_GetSpotsByUser *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetPicturesByNewest(Command_GetPicturesByNewest *command)
{
  QList<Pictures> qList_Pictures = m_DatabaseManager->GetTablePicture()->GetByNewest();
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetPicturesBySpot(Command_GetPicturesBySpot *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetPicturesByUser(Command_GetPicturesByUser *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_UploadPictureToSpot(Command_UploadPictureToSpot *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_UploadNewSpot(Command_UploadNewSpot *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_PictureRemove(Command_PictureRemove *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_SignalDisappearedSpot(Command_SignalDisappearedSpot *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_PictureLike(Command_PictureLike *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_PictureUnlike(Command_PictureUnlike *command)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetServerApplicationVersion(Command_GetServerApplicationVersion *command)
{
  command->SetResponseParameter_ServerApplicationVersion(QCoreApplication::applicationVersion());
}
