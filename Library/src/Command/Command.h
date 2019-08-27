#ifndef COMMAND_H
#define COMMAND_H

// Qt includes -----------------------------
#include <QString>
#include <QJsonObject>

class Command
{
public:

  Command(const QString &name = QString());

  QString GetName() const;

  // Methods for client connection
  QByteArray GetSendCommandData() const;
  void SetFromReceiveResponseData(const QByteArray &data);

  // Methods for server connection
  QByteArray GetSendResponseData() const;
  void SetFromReceiveCommandData(const QByteArray &data);

protected:

  void setResponseParameter(const QString &responseParameterName,
                            const QString &responseParameterString);

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
