#ifndef COMMANDRECEIVER_H
#define COMMANDRECEIVER_H

// Qt includes -----------------------------
#include <QObject>
#include <QMap>

// Class forward declarations --------------
class Command;
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

protected:

  virtual void executeCommand(Command *command) = 0;

private:

  QList<TcpIpServerConnection *> m_QList_TcpIpServerConnection;

  QMap<Command *, TcpIpServerConnection *> m_QMap_Command_TcpIpServerConnection;
};

#endif // COMMANDRECEIVER_H
