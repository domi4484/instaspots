#ifndef TCPIPSERVERCONNECTION_H
#define TCPIPSERVERCONNECTION_H

#include <QTcpSocket>

class TcpIpServerConnection : public QTcpSocket
{

  Q_OBJECT

public:

  explicit TcpIpServerConnection(qintptr socketDescriptor,
                                 QObject *parent = nullptr);

signals:

public slots:
};

#endif // TCPIPSERVERCONNECTION_H
