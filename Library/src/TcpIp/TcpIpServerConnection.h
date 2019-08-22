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

signals:

  void signal_ReceivedData(const QByteArray &qByteArray_Data);

private slots:

  void slot_ReadyRead();

private:

  // Link to CommandReceiver
  CommandReceiver *m_CommandReceiver;

};

#endif // TCPIPSERVERCONNECTION_H
