#ifndef TCPIPSERVER_H
#define TCPIPSERVER_H

// Qt includes -----------------------------
#include <QTcpServer>

// Forward declarations --------------------
class CommandReceiver;

class TcpIpServer : public QTcpServer
{
  Q_OBJECT

public:

  explicit TcpIpServer(CommandReceiver *commandReceiver,
                       QObject *parent = nullptr);
  ~TcpIpServer();

  void StartListening(int port);

protected:

  void incomingConnection(qintptr socketDescriptor) override;

private slots:

  void slot_TcpIpServerConnection_disconnected();

private:

  // Link to CommandReceiver
  CommandReceiver *m_CommandReceiver;

};

#endif // TCPIPSERVER_H
