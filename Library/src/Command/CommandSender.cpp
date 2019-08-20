
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

Command CommandSender::Transceive(const Command &command)
{

}
