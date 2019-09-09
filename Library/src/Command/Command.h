#ifndef COMMAND_H
#define COMMAND_H

// Qt includes -----------------------------
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QDateTime>

class Command : public QObject
{

  Q_OBJECT

public:

  Command(const QString &name = QString(),
          QObject *parent = nullptr);

  QString GetName() const;

  // Methods for client connection
  QByteArray GetSendCommandData() const;
  void SetFromReceiveResponseData(const QByteArray &data);

  // Methods for server connection
  QByteArray GetSendResponseData() const;
  void SetFromReceiveCommandData(const QByteArray &data);

  void SetResponseParameters(const QJsonObject &qJsonObject);
  QJsonObject GetResponseParameters() const;

protected:

  // Set Command Parameters
  void setCommandParameter(const QString &parameterName,
                           int parameterValue);
  void setCommandParameter(const QString &parameterName,
                           const QString &parameterValue);
  void setCommandParameter(const QString &parameterName,
                           const QDateTime &parameterValue);

  // Get Command Parameters
  int getCommandParameter(const QString &parameterName,
                          int parameterValueDefault = 0) const;
  QString getCommandParameter(const QString &parameterName,
                              const QString &parameterValueDefault = QString()) const;
  QDateTime getCommandParameter(const QString &parameterName,
                                const QDateTime &parameterValueDefault = QDateTime()) const;

  // Set Response Parameters
  void setResponseParameter(const QString &parameterName,
                            int parameterValue);
  void setResponseParameter(const QString &parameterName,
                            const QString &parameterValue);
  void setResponseParameter(const QString &parameterName,
                            const QDateTime &parameterValue);

  // Get Response Parameters
  int getResponseParameter(const QString &parameterName,
                           int parameterValueDefault = 0) const;
  QString getResponseParameter(const QString &parameterName,
                               const QString &parameterValueDefault = QString()) const;
  QDateTime getResponseParameter(const QString &parameterName,
                                 const QDateTime &parameterValueDefault = QDateTime()) const;



private:

  // Methods for client connection
  void getValues_Command(QJsonObject &qJsonObject) const;
  void setValues_Response(const QJsonObject &qJsonObject);

  // Methods for server connection
  void getValues_Response(QJsonObject &qJsonObject) const;
  void setValues_Command(const QJsonObject &qJsonObject);

  QString m_Name;

  QJsonObject m_QJsonOject_CommandParameter;
  QJsonObject m_QJsonOject_ResponseParameter;

};

#endif // COMMAND_H
