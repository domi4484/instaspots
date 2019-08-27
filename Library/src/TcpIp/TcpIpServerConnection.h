#ifndef TCPIPSERVERCONNECTION_H
#define TCPIPSERVERCONNECTION_H

// Qt includes -----------------------------
#include <QTcpSocket>

class TcpIpServerConnection : public QTcpSocket
{

  Q_OBJECT

public:

  explicit TcpIpServerConnection(qintptr socketDescriptor,
                                 QObject *parent = nullptr);
  ~TcpIpServerConnection();

  void SendData(const QByteArray &data);

signals:

  void signal_ReceivedData(const QByteArray &qByteArray_Data);

private slots:

  void slot_ReadyRead();

};

#endif // TCPIPSERVERCONNECTION_H
