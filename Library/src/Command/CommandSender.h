#ifndef COMMANDSENDER_H
#define COMMANDSENDER_H

// Project includes ------------------------
#include "Command/Command.h"

// Forward declarations --------------------
class TcpIpClientConnection;

class CommandSender
{
public:

  CommandSender(TcpIpClientConnection *tcpIpClientConnection);

  void Transceive(Command *command);

private:

  // Link to TcpIpClientConnection
  TcpIpClientConnection *m_TcpIpClientConnection;
};

#endif // COMMANDSENDER_H
