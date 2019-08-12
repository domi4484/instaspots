#ifndef TCPIPSERVER_H
#define TCPIPSERVER_H

// Qt includes -----------------------------
#include <QTcpServer>

class TcpIpServer : public QObject
{
  Q_OBJECT

public:

  explicit TcpIpServer(QObject *parent = nullptr);
  ~TcpIpServer();

  void StartListening(int port);

private slots:

private:

  QTcpServer m_QTcpServer;

};

#endif // TCPIPSERVER_H
