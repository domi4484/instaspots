#ifndef COMMANDRECEIVER_H
#define COMMANDRECEIVER_H

// Qt includes -----------------------------
#include <QObject>

// Class forward declarations --------------
class TcpIpServerConnection;

class CommandReceiver : public QObject
{

  Q_OBJECT

public:

  CommandReceiver(QObject *parent = nullptr);

  void AddTcpIpServerConnection(TcpIpServerConnection *tcpIpServerConnection);
  void RemoveTcpIpServerConnection(TcpIpServerConnection *tcpIpServerConnection);

public slots:

  void slot_ReceivedData(const QByteArray &data);

private:

  QList<TcpIpServerConnection *> m_QList_TcpIpServerConnection;
};

#endif // COMMANDRECEIVER_H
