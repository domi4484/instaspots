
// Files includes --------------------------
#include "CommandSender.h"

// Project includes ------------------------
#include "TcpIp/TcpIpClientConnection.h"

//-----------------------------------------------------------------------------------------------------------------------------

CommandSender::CommandSender(TcpIpClientConnection *tcpIpClientConnection)
  : m_TcpIpClientConnection(tcpIpClientConnection)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandSender::Transceive(const Command &command)
{
  m_TcpIpClientConnection->SendData(command.GetSendCommandData());

  qui mettere il comando in una coda finche torna? comandi simultanei -> nuovi sender?
}

//-----------------------------------------------------------------------------------------------------------------------------

