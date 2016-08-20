
// Files includes --------------------------
#include "UltraNetworkAccessManager.h"

// Qt includes -----------------------------
#include <QNetworkRequest>

//-----------------------------------------------------------------------------------------------------------------------------

UltraNetworkAccessManager::UltraNetworkAccessManager(QObject *parent) :
  QNetworkAccessManager(parent)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QNetworkReply *UltraNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                        const QNetworkRequest &request,
                                                        QIODevice *outgoingData)
{
    QNetworkRequest req = request;

    // Ignore ssl certificate
    QSslConfiguration conf = req.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    req.setSslConfiguration(conf);

    QNetworkReply *reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    return reply;
}
