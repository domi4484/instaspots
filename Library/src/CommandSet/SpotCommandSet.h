#ifndef PICTURECOMMANDSET_H
#define PICTURECOMMANDSET_H

// Project includes ------------------------
#include "../Command/Command.h"
#include "../Command/CommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetSpot : public Command
{

public:

  Command_GetSpot()
    : Command(QString("GetSpot"))
  {;}

};

//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetSpotsByDistance : public Command
{

public:

  Command_GetSpotsByDistance()
    : Command(QString("GetSpotsByDistance"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetSpotsByUser : public Command
{

public:

  Command_GetSpotsByUser()
    : Command(QString("GetSpotsByUser"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetPicturesByNewest : public Command
{

public:

  Command_GetPicturesByNewest()
    : Command(QString("GetPicturesByNewest"))
  {;}

  void SetCommandParameter_StartDatetime(const QDateTime &qDateTime_Start);
  QDateTime GetResponseParameter_StartDatetime();

  void SetCommandParameter_Count(int count);
  int GetResponseParameter_Count();

  void SetResponseParameter_PictureList(const QString &serverApplicationVersion);
  QString GetResponseParameter_EntityPictureList();

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetPicturesBySpot : public Command
{

public:

  Command_GetPicturesBySpot()
    : Command(QString("GetPicturesBySpot"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_GetPicturesByUser : public Command
{

public:

  Command_GetPicturesByUser()
    : Command(QString("GetPicturesByUser"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_UploadPictureToSpot : public Command
{

public:

  Command_UploadPictureToSpot()
    : Command(QString("UploadPictureToSpot"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_UploadNewSpot : public Command
{

public:

  Command_UploadNewSpot()
    : Command(QString("UploadNewSpot"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_PictureRemove : public Command
{

public:

  Command_PictureRemove()
    : Command(QString("PictureRemove"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_SignalDisappearedSpot : public Command
{

public:

  Command_SignalDisappearedSpot()
    : Command(QString("SignalDisappearedSpot"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_PictureLike : public Command
{

public:

  Command_PictureLike()
    : Command(QString("PictureLike"))
  {;}

};


//-----------------------------------------------------------------------------------------------------------------------------

class Command_PictureUnlike : public Command
{

public:

  Command_PictureUnlike()
    : Command(QString("PictureUnlike"))
  {;}

};

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

class SpotCommandSet : public CommandSet
{

public:

  SpotCommandSet();

};

#endif // PICTURECOMMANDSET_H
