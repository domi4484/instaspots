
// Files includes --------------------------
#include "TcpIpServerConnection.h"

// Library includes ------------------------
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QHostAddress>

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServerConnection::TcpIpServerConnection(qintptr socketDescriptor,
                                             CommandReceiver *commandReceiver,
                                             QObject *parent)
  : QTcpSocket(parent)
  , m_CommandReceiver(commandReceiver)
{
  QTcpSocket::setSocketDescriptor(socketDescriptor);

  Logger::debug(QString("TcpIpServerConnection new connection from '%1'").arg(QTcpSocket::peerAddress().toString()));
}

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServerConnection::~TcpIpServerConnection()
{
  Logger::debug(QString("TcpIpServerConnection closed connection from '%1'").arg(QTcpSocket::peerAddress().toString()));
}
