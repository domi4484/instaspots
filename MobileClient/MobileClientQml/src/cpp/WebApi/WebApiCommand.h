/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      29.10.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef WEBAPICOMMAND_H
#define WEBAPICOMMAND_H

// Project includes ------------------------
#include "WebApiError.h"
#include "QueryItem.h"

// Qt includes -----------------------------
#include <QObject>
#include <QMap>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Forward declarations --------------------
class QIODevice;

class WebApiCommand : public QObject
{
  Q_OBJECT

public:

  enum EnumRequestType
  {
    RequestTypePost,
    RequestTypeGet
  };

  enum EnumAnswerType
  {
    AnswerTypeJSON,
    AnswerTypeBinary
  };

  explicit WebApiCommand(QObject *parent = 0);

  void setCommandName(const QString &commandName);
  QString commandName() const;

  // Request
  QVariant requestParameter(const QString &parameterName) const;

  // Result (JSON)
  void setResult(const WebApiError &error,
                 const QJsonDocument &result);
  QVariant   resultParameter(const QString &parameterName);
  QJsonValue resultProperty(const QString &propertyName);
  QJsonArray resultArray();
  QJsonObject resultObject();

  // RawResult (BINARY)
  virtual void setRawResult(const WebApiError &error,
                            const QByteArray &result);
  QByteArray rawResult();

  virtual void setProgress(qint64 progress,
                           qint64 total);

  void setRequestType(EnumRequestType requestType);
  EnumRequestType requestType() const;

  void setAnswerType(EnumAnswerType type);
  virtual EnumAnswerType answerType() const;

  bool isRunning();

  WebApiError sendRequest(QList<QueryItem> &queryItems,
                          QIODevice *device = NULL);

  QString requestString() const;

  WebApiError error() const;
  QString errorString() const;

signals:

  void signal_Finished();
  void signal_Finished(const WebApiError &error);
  void signal_Progress(qint64 progress,
                       qint64 total);

private:

  QString mCommandName;
  EnumRequestType mRequestType;
  EnumAnswerType mAnswerType;
  QMap<QString, QueryItem> mQMapQueryItems;

  bool m_Running;

  QJsonDocument m_JsonDocumentResult;
  QByteArray m_RawResult;

  WebApiError m_Error;
};

#endif // WEBAPICOMMAND_H
