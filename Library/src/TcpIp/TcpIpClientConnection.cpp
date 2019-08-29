
// Files includes --------------------------
#include "TcpIpClientConnection.h"

// Project includes ------------------------
#include "HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QHostAddress>

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpClientConnection::TcpIpClientConnection(QObject *parent)
  : QObject(parent)
  , m_QTcpSocket(this)
{
  QObject::connect(&m_QTcpSocket,
                   SIGNAL(readyRead()),
                   this,
                   SLOT(slot_QTcpSocket_ReadyRead()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpClientConnection::Connect(const QString &addresse,
                                    int port)
{
  m_QTcpSocket.connectToHost(QHostAddress(addresse),
                             port);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool TcpIpClientConnection::WaitForConnected(int timeoutMs)
{
  return m_QTcpSocket.waitForConnected(timeoutMs);
}

//-----------------------------------------------------------------------------------------------------------------------------

bool TcpIpClientConnection::IsConnected()
{
  return m_QTcpSocket.state() == QTcpSocket::ConnectedState;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpClientConnection::SendData(const QByteArray &data)
{
  if(m_QTcpSocket.write(data) < 0)
    throw Exception(QString("QTcpSocket can't send data. Error: '%1'")
                    .arg(m_QTcpSocket.errorString()));
}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpClientConnection::slot_QTcpSocket_ReadyRead()
{
  emit signal_ReceivedData(m_QTcpSocket.readAll());
}
