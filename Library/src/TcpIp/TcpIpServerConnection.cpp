
// Files includes --------------------------
#include "TcpIpServerConnection.h"

// Library includes ------------------------
#include <HelperClasses/Logger.h>

// Qt includes -----------------------------
#include <QHostAddress>

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServerConnection::TcpIpServerConnection(qintptr socketDescriptor,
                                             QObject *parent)
  : QTcpSocket(parent)
{
  QTcpSocket::setSocketDescriptor(socketDescriptor);

  QObject::connect(this,
                   SIGNAL(readyRead()),
                   SLOT(slot_ReadyRead()));

  Logger::debug(QString("TcpIpServerConnection new connection from '%1'").arg(QTcpSocket::peerAddress().toString()));
}

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServerConnection::~TcpIpServerConnection()
{
  Logger::debug(QString("TcpIpServerConnection closed connection from '%1'").arg(QTcpSocket::peerAddress().toString()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServerConnection::slot_ReadyRead()
{
  emit signal_ReceivedData(QTcpSocket::readAll());
}
