
// Files includes --------------------------
#include "CommandReceiver.h"

// Library includes ------------------------
#include "HelperClasses/Logger.h"
#include "TcpIp/TcpIpServerConnection.h"

//-----------------------------------------------------------------------------------------------------------------------------

CommandReceiver::CommandReceiver(QObject *parent)
  : QObject(parent)
  , m_QList_TcpIpServerConnection()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandReceiver::AddTcpIpServerConnection(TcpIpServerConnection *tcpIpServerConnection)
{
  m_QList_TcpIpServerConnection.append(tcpIpServerConnection);
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandReceiver::RemoveTcpIpServerConnection(TcpIpServerConnection *tcpIpServerConnection)
{
  m_QList_TcpIpServerConnection.removeOne(tcpIpServerConnection);
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandReceiver::slot_ReceivedData(const QByteArray &data)
{
  // new command , decode und in liste, poi delete later e via dalla lista quando viene risposto
  Logger::info(QString("CommandReceiver Message received: %1").arg(QString::fromLatin1(data)));
}
