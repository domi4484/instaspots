
// Files includes --------------------------
#include "SpotCommandSet.h"

//-----------------------------------------------------------------------------------------------------------------------------

SpotCommandSet::SpotCommandSet()
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
