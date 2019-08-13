#ifndef TCPIPSERVER_H
#define TCPIPSERVER_H

// Qt includes -----------------------------
#include <QTcpServer>

class TcpIpServer : public QTcpServer
{
  Q_OBJECT

public:

  explicit TcpIpServer(QObject *parent = nullptr);
  ~TcpIpServer();

  void StartListening(int port);

protected:

  void incomingConnection(qintptr socketDescriptor) override;

};

#endif // TCPIPSERVER_H
