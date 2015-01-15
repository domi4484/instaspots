/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      29.10.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

// Project includes ------------------------
#include "WebApiError.h"
#include "QueryItem.h"

// Qt includes -----------------------------
#include <QObject>
#include <QByteArray>
#include <QScriptValue>

// Forward declarations --------------------
class QIODevice;

class WebApiCommand : public QObject
{
  Q_OBJECT

public:

  enum ANSWER_TYPE
  {
    JSON,
    BINARY
  };

  explicit WebApiCommand(QObject *parent = 0);

  void setCommand(const QString &command);
  QString command() const;

  // Result (JSON)
  virtual void setResult(const WebApiError &error,
                         const QScriptValue &result);
  QVariant     resultParameter(const QString &parameterName);
  QScriptValue resultProperty(const QString &propertyName);

  // RawResult (BINARY)
  virtual void setRawResult(const WebApiError &error,
                            const QByteArray &result);
  QByteArray rawResult();

  virtual void setProgress(qint64 progress,
                           qint64 total);

  void setAnswerType(ANSWER_TYPE type);
  virtual ANSWER_TYPE answerType() const { return m_AnswerType; }

  bool isRunning() { return m_Running; }

  void postRequest(const QList<QueryItem> &queryItems,
                   QIODevice *device = NULL);

signals:

  void signal_Finished(const WebApiError &error);
  void signal_Progress(qint64 progress,
                       qint64 total);

public slots:

private:

  QString     m_Command;
  ANSWER_TYPE m_AnswerType;

  bool m_Running;

  QScriptValue m_Result;
  QByteArray   m_RawResult;

};

#endif // ABSTRACTCOMMAND_H
