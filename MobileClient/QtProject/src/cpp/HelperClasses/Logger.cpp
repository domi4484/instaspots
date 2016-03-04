/********************************************************************
 *                                                                 *
 * Insta Spots                                                     *
 *                                                                 *
 * Author:                                                         *
 * Created:      --.--.----                                        *
 *                                                                 *
********************************************************************/

// Files includes
#include "Logger.h"

// Qt includes
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QThread>

//-----------------------------------------------------------------------------------------------------------------------------

const int Logger::_CONST::MAX_LOG_LINES(1000);

//-----------------------------------------------------------------------------------------------------------------------------

Logger *Logger::s_Logger = NULL;
QMutex *const Logger::s_Mutex = new QMutex;

//-----------------------------------------------------------------------------------------------------------------------------

Logger::Logger(LOG_LEVEL logLevel,
               QObject *parent) :
  QObject(parent),
  m_LogLevel(qMax (LOG_INFO, logLevel)),
  m_QStringList_LogEntries()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Logger::~Logger()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::instanziate(Logger::LOG_LEVEL logLevel)
{
  if(s_Logger != NULL)
  {
    qWarning() << "Logger is already initialised.";
    return;
  }

  s_Logger = new Logger(logLevel);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::destroy()
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger instance is already null.";
    return;
  }

  delete s_Logger;
  s_Logger = NULL;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::error(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_ERROR, text);
}

void Logger::warning(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_WARNING, text);
}

void Logger::info(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_INFO, text);
}

void Logger::trace(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_TRACE, text);
}

void Logger::debug(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_DEBUG, text);
}

void Logger::verbose(const QString &text)
{
  if(s_Logger == NULL)
  {
    qWarning() << "Logger have to be initialised before use.";
    return;
  }

  s_Logger->write (LOG_VERBOSE, text);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::setLogLevel(Logger::LOG_LEVEL log_level)
{
  m_LogLevel = log_level;
}

//-----------------------------------------------------------------------------------------------------------------------------

Logger::LOG_LEVEL Logger::getLogLevel()
{
  return m_LogLevel;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::slot_error(const QString &text)
{
  write (LOG_ERROR, text);
}

void Logger::slot_warning(const QString &text)
{
  write (LOG_WARNING, text);
}

void Logger::slot_info(const QString &text)
{
  write (LOG_INFO, text);
}

void Logger::slot_trace(const QString &text)
{
  write (LOG_TRACE, text);
}

void Logger::slot_debug(const QString &text)
{
  write (LOG_DEBUG, text);
}

void Logger::slot_verbose(const QString &text)
{
  write (LOG_VERBOSE, text);
}

//-----------------------------------------------------------------------------------------------------------------------------

void Logger::write (LOG_LEVEL log_level,
                    const QString &text)
{
   QMutexLocker mutexLock (s_Mutex);

   if (log_level <= m_LogLevel)
   {
      const QString prefix = QString ("%1 %2 ")
                             .arg (QDateTime::currentDateTime().toString ("MM.dd hh:mm:ss.zzz"))
                             .arg (toString (log_level));
      QString txt = text;
      txt.remove("\n");

      QString logText(prefix + txt);

      m_QStringList_LogEntries.prepend(logText);
      while(m_QStringList_LogEntries.size() >= _CONST::MAX_LOG_LINES)
      {
         m_QStringList_LogEntries.removeLast();
      }

      qDebug() << logText;
   }
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Logger::toString (LOG_LEVEL log_level)
{
   switch (log_level)
   {
   case LOG_ERROR:   return "E";
   case LOG_WARNING: return "W";
   case LOG_INFO:    return "I";
   case LOG_TRACE:   return "T";
   case LOG_DEBUG:   return "D";
   case LOG_VERBOSE: return "V";
   }

   return QString ("LOG_LEVEL (%1)").arg (log_level);
}
