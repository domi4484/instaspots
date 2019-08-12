#ifndef SETTINGS_H
#define SETTINGS_H

// Qt includes -----------------------------
#include <QFileInfo>
#include <QString>
#include <QMap>

class Settings
{
public:

  class _CONST
  {
  public:
    static const QString FILENAME_EXTENSION;
  }; // _CONST

  Settings();
  void Load(const QFileInfo &qFileInfo);
  void Write(const QString &filename = QString());
  QFileInfo getFileInfo() const;

  class Values
  {
  public:

    class General
    {
    public:

      QString m_ApplicationName;
      QString m_ApplicationVersion;

    }; // General

    General general;

    class Logger
    {
    public:

      enum Enum_LogLevel
      {
         LogLevel_Error    = 0,   //!< An (unrecoverable) error has occurred. User intervention is mandatory.
         LogLevel_Warning  = 1,   //!< An error has occurred, which does not stop the device from operation. User intervention is optional.
         LogLevel_Info     = 2,    //!< A normal operation event has occurred.
         LogLevel_Trace    = 3,    //!< A detailed operation event has occurred.
         LogLevel_Debug    = 4,    //!< A more detailed operation event has occurred.
         LogLevel_Verbose  = 5     //!< A verbose operation (i.e. device command, SQL) has occurred.
      };

      Enum_LogLevel m_LogLevel;

    }; // Logger

    Logger logger;

    class TcpIpServer
    {
    public:

      int m_Port;

    }; // TcpIpServer

    TcpIpServer tcpIpServer;

  }; // Values

  QString Get_Values_General_ApplicationName() const;
  QString Get_Values_General_ApplicationVersion() const;

  Values::Logger::Enum_LogLevel Get_Values_Logger_LogLevel() const;

  int Get_Values_TcpIpServer_Port() const;

private:

  void createDefault_Values();
  void createDefault_Values_General();
  void createDefault_Values_Logger();
  void createDefault_Values_TcpIpServer();

  void load_Values(const QJsonObject &qJsonObject);
  void load_Values_General(const QJsonObject &qJsonObject);
  void load_Values_Logger(const QJsonObject &qJsonObject);
  void load_Values_TcpIpServer(const QJsonObject &qJsonObject);

  void write_Values(QJsonObject &qJsonObject);
  void write_Values_General(QJsonObject &qJsonObject);
  void write_Values_Logger(QJsonObject &qJsonObject);
  void write_Values_TcpIpServer(QJsonObject &qJsonObject);

  QFileInfo m_QFileInfo;

  QString m_Name;

  Values m_Values;

};

#endif // SETTINGS_H
