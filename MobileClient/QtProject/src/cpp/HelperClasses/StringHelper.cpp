#include <QString>

#include "StringHelper.h"

//-----------------------------------------------------------------------------------------------------------------------------

QString StringHelper::quoted (const QString &string)
{
   QString str = string;
   return "\"" +  str.replace ('\\', "\\\\").replace ('"', "\\\"").replace ('\n', "\\n") + "\"";
}

//-----------------------------------------------------------------------------------------------------------------------------

QString StringHelper::doubleQuote(const QString &string)
{
   QString str = string;
   return "\"" + str.replace('\"', "\"\"") + "\"";
}

//-----------------------------------------------------------------------------------------------------------------------------

QString StringHelper::sqlQuote(const QString &string)
{
   QString str = string;
   return "'" + str.replace('\'', "''") + "'";
}
