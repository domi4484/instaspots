#ifndef COMMAND_H
#define COMMAND_H

// Qt includes -----------------------------
#include <QString>
#include <QJsonObject>

class Command
{
public:

  Command(const QString &name = QString());

  QByteArray GetSendCommandData() const;

private:

  void getValues_Command(QJsonObject &qJsonObject) const;
  void getValues_CommandParameters(QJsonObject &qJsonObject) const;

  QString m_Name;

};

#endif // COMMAND_H
