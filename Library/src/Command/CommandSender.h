#ifndef COMMANDSENDER_H
#define COMMANDSENDER_H

// Project includes ------------------------
#include "Command/Command.h"

// Qt includes -----------------------------
#include <QObject>

// Forward declarations --------------------
class TcpIpClientConnection;

class CommandSender : public QObject
{

  Q_OBJECT

public:

  CommandSender(TcpIpClientConnection *tcpIpClientConnection,
                QObject *parent = nullptr);

  void Send(Command *command);

private slots:

  void slot_ReceivedData(const QByteArray &data);
  void slot_Command_Destroyed(QObject *command);

private:

  // Link to TcpIpClientConnection
  TcpIpClientConnection *m_TcpIpClientConnection;

  // Commands running
  QList<Command *> m_QList_Commands;
};

#endif // COMMANDSENDER_H
