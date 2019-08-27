
// Files includes --------------------------
#include "TcpIpServerConnection.h"

// Library includes ------------------------
#include <HelperClasses/Exception.h>
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

void TcpIpServerConnection::SendData(const QByteArray &data)
{
  if(QTcpSocket::write(data) < 0)
    throw Exception(QString("QTcpSocket can't send data. Error: '%1'")
                           .arg(QTcpSocket::errorString()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServerConnection::slot_ReadyRead()
{
  emit signal_ReceivedData(QTcpSocket::readAll());
}
