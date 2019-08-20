
// Files includes --------------------------
#include "TcpIpServer.h"

// Project includes ------------------------
#include "TcpIpServerConnection.h"
#include "../HelperClasses/Exception.h"
#include "../HelperClasses/Logger.h"

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServer::TcpIpServer(CommandReceiver *commandReceiver,
                         QObject *parent)
  : QTcpServer(parent)
  , m_CommandReceiver(commandReceiver)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServer::~TcpIpServer()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServer::StartListening(int port)
{
  if(QTcpServer::listen(QHostAddress::Any,
                        port)
     == false)
  {
    throw Exception(QString("QTcpServer can't listen to port '%1'; Error: '%2'")
                           .arg(port)
                           .arg(QTcpServer::errorString()));
  }

  Logger::info(QString("TcpIpServer started listening on port '%1'").arg(port));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServer::incomingConnection(qintptr socketDescriptor)
{
  TcpIpServerConnection *tcpIpServerConnection = new TcpIpServerConnection(socketDescriptor,
                                                                           m_CommandReceiver,
                                                                           this);
  QObject::connect(tcpIpServerConnection,
                   SIGNAL(disconnected()),
                   tcpIpServerConnection,
                   SLOT(deleteLater()));
}
