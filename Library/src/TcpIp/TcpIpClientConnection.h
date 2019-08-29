#ifndef TCPIPCLIENTCONNECTION_H
#define TCPIPCLIENTCONNECTION_H

// Qt includes -----------------------------
#include <QObject>
#include <QTcpSocket>

class TcpIpClientConnection : public QObject
{
  Q_OBJECT

public:

  explicit TcpIpClientConnection(QObject *parent = nullptr);

  void Connect(const QString &addresse,
               int port);
  bool WaitForConnected(int timeoutMs);
  bool IsConnected();

  void SendData(const QByteArray &data);

signals:

  void signal_ReceivedData(const QByteArray &qByteArray_Data);

private slots:

  void slot_QTcpSocket_ReadyRead();

private:

  QTcpSocket m_QTcpSocket;
};

#endif // TCPIPCLIENTCONNECTION_H
