
// Files includes --------------------------
#include "SpotCommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------


QJsonObject CommandParameter_Picture::ToJsonObject() const
{
  return QJsonObject();
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandParameter_Picture::FromJsonObject(const QJsonObject &qJsonObject)
{

}

//-----------------------------------------------------------------------------------------------------------------------------


QJsonObject CommandParameter_PictureList::ToJsonObject() const
{
  return QJsonObject();
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandParameter_PictureList::FromJsonObject(const QJsonObject &qJsonObject)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_GetPicturesByNewest::SetCommandParameter_StartDatetime(const QDateTime &qDateTime_Start)
{
  Command::setCommandParameter("StartDateTime",
                               qDateTime_Start);
}

//-----------------------------------------------------------------------------------------------------------------------------

QDateTime Command_GetPicturesByNewest::GetResponseParameter_StartDatetime()
{
  return Command::getCommandParameter("StartDateTime",
                                      QDateTime::currentDateTime());
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_GetPicturesByNewest::SetCommandParameter_Count(int count)
{
  Command::setCommandParameter("Count",
                               count);
}

//-----------------------------------------------------------------------------------------------------------------------------

int Command_GetPicturesByNewest::GetResponseParameter_Count()
{
  return Command::getCommandParameter("Count",
                                      20);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Command_GetPicturesByNewest::SetResponseParameter_PictureList(const CommandParameter_PictureList &commandParameter_PictureList)
{
  Command::setResponseParameter("PictureList",
                                commandParameter_PictureList.ToJsonObject());
}

//-----------------------------------------------------------------------------------------------------------------------------

CommandParameter_PictureList Command_GetPicturesByNewest::GetResponseParameter_EntityPictureList()
{
  CommandParameter_PictureList commandParameter_PictureList;
  commandParameter_PictureList.FromJsonObject(Command::getResponseParameter("PictureList",
                                                                            QJsonObject()));
  return commandParameter_PictureList;
}

//-----------------------------------------------------------------------------------------------------------------------------

SpotCommandSet::SpotCommandSet()
{

}

