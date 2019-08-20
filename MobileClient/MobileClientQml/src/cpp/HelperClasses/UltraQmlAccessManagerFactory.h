#ifndef ULTRAQMLACCESSMANAGERFACTORY_H
#define ULTRAQMLACCESSMANAGERFACTORY_H

#include <QQmlNetworkAccessManagerFactory>
#include <QObject>
#include <QNetworkReply>
#include <QList>
#include <QSslError>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QSslCertificate>

class UltraQmlAccessManagerFactory : public QObject,
                                     public QQmlNetworkAccessManagerFactory
{
  Q_OBJECT

public:

  explicit UltraQmlAccessManagerFactory();

  virtual QNetworkAccessManager* create(QObject* parent);

public slots:

  void onIgnoreSslErrors(QNetworkReply* reply,
                         QList<QSslError> errors);
};

#endif // ULTRAQMLACCESSMANAGERFACTORY_H

