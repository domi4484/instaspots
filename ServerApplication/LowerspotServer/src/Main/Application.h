/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

// Qt includes -----------------------------
#include <QCoreApplication>
#include <QUrl>

// Forward declarations --------------------
class Settings;
class ApplicationCommandReceiver;
class TcpIpServer;
class DatabaseManager;

class Application : public QCoreApplication
{
    Q_OBJECT
public:

  class _CONST
  {
  public:
    class LOGGER
    {
    public:
      static const QString DIRECTORY;
    }; // LOGGER
    class SETTINGS
    {
    public:
      static const QString DIRECTORY;
      static const QString FILENAME;
    }; // SETTINGS
  }; // _CONST

  explicit Application(int argc,
                       char *argv[]);
  ~Application();

private:

  void startupApplication_Logger();
  void startupApplication_Settings();
  void startupApplication_DatabaseManager();
  void startupApplication_CommandReceiver();
  void startupApplication_TcpIpServer();

  // Settings
  Settings *m_Settings;

  // Database manager
  DatabaseManager *m_DatabaseManager;

  // CommandSet
  ApplicationCommandReceiver *m_ApplicationCommandReceiver;

  // TcpIpServer
  TcpIpServer *m_TcpIpServer;
};

#endif // APPLICATION_H
