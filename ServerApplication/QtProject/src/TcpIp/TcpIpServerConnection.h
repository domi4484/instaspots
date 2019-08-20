#ifndef TCPIPSERVERCONNECTION_H
#define TCPIPSERVERCONNECTION_H

// Qt includes -----------------------------
#include <QTcpSocket>

// Forward declarations --------------------
class CommandReceiver;

class TcpIpServerConnection : public QTcpSocket
{

  Q_OBJECT

public:

  explicit TcpIpServerConnection(qintptr socketDescriptor,
                                 CommandReceiver *commandReceiver,
                                 QObject *parent = nullptr);
  ~TcpIpServerConnection();

private:

  // Link to CommandReceiver
  CommandReceiver *m_CommandReceiver;

};

#endif // TCPIPSERVERCONNECTION_H
