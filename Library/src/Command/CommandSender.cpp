
// Files includes --------------------------
#include "CommandSender.h"

// Project includes ------------------------
#include "TcpIp/TcpIpClientConnection.h"

//-----------------------------------------------------------------------------------------------------------------------------

CommandSender::CommandSender(TcpIpClientConnection *tcpIpClientConnection)
  : m_TcpIpClientConnection(tcpIpClientConnection)
{
  connetto m_TcpIpClientConnection con un slot recevi risposta
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandSender::Transceive(Command *command)
{
  qui inserisco il command in una lista

  connetto command signal destroyed per ev rimuoverlo dalla lista

  m_TcpIpClientConnection->SendData(command.GetSendCommandData());
}

//-----------------------------------------------------------------------------------------------------------------------------

