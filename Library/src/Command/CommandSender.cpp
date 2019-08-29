
// Files includes --------------------------
#include "CommandSender.h"

// Project includes ------------------------
#include "HelperClasses/Logger.h"
#include "TcpIp/TcpIpClientConnection.h"

//-----------------------------------------------------------------------------------------------------------------------------

CommandSender::CommandSender(TcpIpClientConnection *tcpIpClientConnection,
                             QObject *parent)
  : QObject(parent)
  , m_TcpIpClientConnection(tcpIpClientConnection)
  , m_QList_Commands()
{
  QObject::connect(m_TcpIpClientConnection,
                   SIGNAL(signal_ReceivedData(const QByteArray &)),
                   this,
                   SLOT(slot_ReceivedData(const QByteArray &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandSender::Send(Command *command)
{
  m_TcpIpClientConnection->SendData(command->GetSendCommandData());

  QObject::connect(command,
                   SIGNAL(destroyed(QObject *)),
                   this,
                   SLOT(slot_Command_Destroyed(QObject *)));

  m_QList_Commands.append(command);
}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandSender::slot_ReceivedData(const QByteArray &data)
{
  // new command , decode e cerca in lista e rispondi
  Logger::info(QString("CommandSender Message received: %1").arg(QString::fromLatin1(data)));

  Command command;
  command.SetFromReceiveResponseData(data);


}

//-----------------------------------------------------------------------------------------------------------------------------

void CommandSender::slot_Command_Destroyed(QObject *command)
{
  m_QList_Commands.removeAll((Command *)command);
}

//-----------------------------------------------------------------------------------------------------------------------------

