#ifndef SERVERAPPLICATIONCOMMANDRECEIVER_H
#define SERVERAPPLICATIONCOMMANDRECEIVER_H

// Library includes ------------------------
#include <Command/CommandReceiver.h>
#include <CommandSet/SpotCommandSet.h>
#include <CommandSet/ApplicationCommandSet.h>

class ApplicationCommandReceiver : public CommandReceiver
{

public:

  ApplicationCommandReceiver();

protected:

  virtual void executeCommand(Command *command);

private:

  // Spot commands
  void executeCommand_GetSpot(Command_GetSpot *command);
  void executeCommand_GetSpotsByDistance(Command_GetSpotsByDistance *command);
  void executeCommand_GetSpotsByUser(Command_GetSpotsByUser *command);
  void executeCommand_GetPicturesByNewest(Command_GetPicturesByNewest *command);
  void executeCommand_GetPicturesBySpot(Command_GetPicturesBySpot *command);
  void executeCommand_GetPicturesByUser(Command_GetPicturesByUser *command);
  void executeCommand_UploadPictureToSpot(Command_UploadPictureToSpot *command);
  void executeCommand_UploadNewSpot(Command_UploadNewSpot *command);
  void executeCommand_PictureRemove(Command_PictureRemove *command);
  void executeCommand_SignalDisappearedSpot(Command_SignalDisappearedSpot *command);
  void executeCommand_PictureLike(Command_PictureLike *command);
  void executeCommand_PictureUnlike(Command_PictureUnlike *command);

  // Application commands
  void executeCommand_GetServerApplicationVersion(Command_GetServerApplicationVersion *command);

};

#endif // SERVERAPPLICATIONCOMMANDRECEIVER_H
