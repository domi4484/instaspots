
// Files includes --------------------------
#include "CommandReceiver.h"

// Library includes ------------------------
#include "Command.h"
#include "HelperClasses/Exception.h"
#include "HelperClasses/Logger.h"
#include "TcpIp/TcpIpServerConnection.h"

//-----------------------------------------------------------------------------------------------------------------------------

CommandReceiver::CommandReceiver(QObject *parent)
  : QObject(parent)
  , m_QList_TcpIpServerConnection()
  , m_QMap_Command_TcpIpServerConnection()
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
  Command *command = new Command();

  try
  {
    command->SetFromReceiveCommandData(data);
  }
  catch (const Exception &exception)
  {
    Logger::info(QString("Can't decode message received: %1").arg(QString::fromLatin1(data)));
    delete command;
    return;
  }

  m_QMap_Command_TcpIpServerConnection.insert(command,
                                             (TcpIpServerConnection *)QObject::sender());

  // Questo farlo asincrono un giorno
  if(executeCommand(command) == false)
  {

  }

  m_QMap_Command_TcpIpServerConnection.value(command)->SendData(command->GetSendResponseData());

  m_QMap_Command_TcpIpServerConnection.remove(command);
  delete command;
}
