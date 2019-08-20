#ifndef ULTRANETWORKACCESSMANAGER_H
#define ULTRANETWORKACCESSMANAGER_H

// Qt includes -----------------------------
#include <QNetworkAccessManager>

class UltraNetworkAccessManager : public QNetworkAccessManager
{
public:
  UltraNetworkAccessManager(QObject *parent = 0);

  QNetworkReply *createRequest(QNetworkAccessManager::Operation op,
                               const QNetworkRequest &request,
                               QIODevice *outgoingData);
};

#endif // ULTRANETWORKACCESSMANAGER_H
