
// Files includes --------------------------
#include "ApplicationCommandReceiver.h"
#include "Entity/EntityPicture.h"
#include "Database/DatabaseManager.h"
#include "Database/DatabaseTablePicture.h"

// Library includes ------------------------
#include <HelperClasses/Exception.h>
#include <HelperClasses/Logger.h>

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
  QList<EntityPicture> qList_EntityPicture = m_DatabaseManager->GetTablePicture()->GetByNewest(QDateTime::currentDateTime(),
                                                                                               20);

  foreach (const EntityPicture &entityPicture, qList_EntityPicture)
  {
    Logger::info(QString("Pic url %1").arg(entityPicture.Get_Url()));
  }
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

//  // Add score to user
//  $user->addReputation(10);

}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_UploadNewSpot(Command_UploadNewSpot *command)
{

//  // Add score to user
//  $user->addReputation(10);

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
//  public function addLiker(\Instaspots\UserBundle\Entity\User $liker)
//      {
//        // Check if already liked
//        if($this->likers->contains($liker))
//          return $this;

//        // Assign reputation point to liked user
//        if($this->user != $liker)
//        {
//          $this->user->notifyLikedPicture($this);
//        }

//        // Assign score point to spot
//        $this->spot->addScore(1);

//        // Append liker
//        $this->likers[] = $liker;

//        return $this;
//      }
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_PictureUnlike(Command_PictureUnlike *command)
{
//  /**
//   * Remove likers
//   *
//   * @param \Instaspots\UserBundle\Entity\User $likers
//   */
//  public function removeLiker(\Instaspots\UserBundle\Entity\User $liker)
//  {
//    // Check if already unliked
//    if($this->likers->contains($liker) == false)
//      return $this;

//    // Remove reputation point to liked user
//    if($this->user != $liker)
//    {
//      $this->user->removeReputation(1);
//    }

//    // Remove score point from spot
//    $this->spot->removeScore(1);

//    // Remove liker
//    $this->likers->removeElement($liker);
//  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationCommandReceiver::executeCommand_GetServerApplicationVersion(Command_GetServerApplicationVersion *command)
{
  command->SetResponseParameter_ServerApplicationVersion(QCoreApplication::applicationVersion());
}
