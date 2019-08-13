
// Files includes --------------------------
#include "TcpIpServerConnection.h"

// Project includes ------------------------
#include "../HelperClasses/Logger.h"

// Qt includes -----------------------------
#include <QHostAddress>

TcpIpServerConnection::TcpIpServerConnection(qintptr socketDescriptor,
                                             QObject *parent)
  : QTcpSocket(parent)
{
  QTcpSocket::setSocketDescriptor(socketDescriptor);

  Logger::debug(QString("TcpIpServerConnection new connection from '%1'").arg(QTcpSocket::peerAddress().toString()));
}
