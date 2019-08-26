
// Files includes --------------------------
#include "TcpIpServer.h"

// Library includes ------------------------
#include "TcpIpServerConnection.h"
#include "Command/CommandReceiver.h"
#include "HelperClasses/Exception.h"
#include "HelperClasses/Logger.h"

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
                                                                           this);
  m_CommandReceiver->AddTcpIpServerConnection(tcpIpServerConnection);
  QObject::connect(tcpIpServerConnection,
                   SIGNAL(signal_ReceivedData(const QByteArray &)),
                   m_CommandReceiver,
                   SLOT(slot_ReceivedData(const QByteArray &)));
  QObject::connect(tcpIpServerConnection,
                   SIGNAL(disconnected()),
                   this,
                   SLOT(slot_TcpIpServerConnection_disconnected()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServer::slot_TcpIpServerConnection_disconnected()
{
  TcpIpServerConnection *tcpIpServerConnection = (TcpIpServerConnection *)QObject::sender();

  m_CommandReceiver->RemoveTcpIpServerConnection(tcpIpServerConnection);
  tcpIpServerConnection->deleteLater();
}
