
// File includes ---------------------------
#include "UltraQmlAccessManagerFactory.h"

// Project includes ------------------------
#include "UltraNetworkAccessManager.h"

//-----------------------------------------------------------------------------------------------------------------------------

UltraQmlAccessManagerFactory::UltraQmlAccessManagerFactory() :
  QObject(0),
  QQmlNetworkAccessManagerFactory()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

QNetworkAccessManager* UltraQmlAccessManagerFactory::create(QObject* parent)
{
  UltraNetworkAccessManager *qNetworkAccessManager = new UltraNetworkAccessManager(parent);
  QObject::connect(qNetworkAccessManager,
                   SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                   this,
                   SLOT(onIgnoreSslErrors(QNetworkReply*,QList<QSslError>)));

  return qNetworkAccessManager;
}

//-----------------------------------------------------------------------------------------------------------------------------

void UltraQmlAccessManagerFactory::onIgnoreSslErrors(QNetworkReply *reply,
                                                     QList<QSslError> errors)
{
  reply->ignoreSslErrors(errors);
}



