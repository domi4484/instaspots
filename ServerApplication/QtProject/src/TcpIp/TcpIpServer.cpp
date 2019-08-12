
// Files includes --------------------------
#include "TcpIpServer.h"

// Project includes ------------------------
#include "../HelperClasses/Exception.h"
#include "../HelperClasses/Logger.h"

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServer::TcpIpServer(QObject *parent)
  : QObject(parent)
  , m_QTcpServer(this)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

TcpIpServer::~TcpIpServer()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void TcpIpServer::StartListening(int port)
{
  if(m_QTcpServer.listen(QHostAddress::Any,
                         port)
     == false)
  {
    throw Exception(QString("QTcpServer can't listen to port '%1'; Error: '%2'")
                           .arg(port)
                           .arg(m_QTcpServer.errorString()));
  }

  Logger::info(QString("TcpIpServer started listening on port '%1'").arg(port));
}
