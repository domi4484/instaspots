#ifndef COMMAND_H
#define COMMAND_H

// Qt includes -----------------------------
#include <QString>
#include <QJsonObject>

class Command
{
public:

  Command(const QString &name = QString());

  // Methods for client connection
  QByteArray GetSendCommandData() const;
  void SetFromReceiveResponseData(const QByteArray &data);

  // Methods for server connection
  QByteArray GetSendResponseData() const;
  void SetFromReceiveCommandData(const QByteArray &data);

private:

  // Methods for client connection
  void getValues_Command(QJsonObject &qJsonObject) const;
  void getValues_CommandParameters(QJsonObject &qJsonObject) const;

  // Methods for server connection
  void setValues_Command(const QJsonObject &qJsonObject);

  QString m_Name;

};

#endif // COMMAND_H
