
#include "TcpIpClientConnection.h"

// Qt includes -----------------------------
#include <QHostAddress>

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpClientConnection::TcpIpClientConnection(QObject *parent)
  : QObject(parent)
  , m_QTcpSocket(this)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpClientConnection::Connect(const QString &addresse,
                                    int port)
{
  if(m_QTcpSocket.bind(QHostAddress(addresse),
                       port)
     == false)
  {
    throw QString(QString("QTcpSocket can't bind to server '%1:%2'; Error: '%3'")
                           .arg(addresse)
                           .arg(port)
                           .arg(m_QTcpSocket.errorString()));
  }
}
