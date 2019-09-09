#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


// Qt includes --------------------------------------------
#include <QObject>
#include <QSqlDatabase>

class DatabaseTablePicture;

class DatabaseManager : public QObject
{
  Q_OBJECT

public:

  class _CONST
  {
  public:
    static const QString DATABASE_FILENAME_EXTENSION;

    static const QString TIMESTAMP_FORMAT;
  }; // _CONST

  explicit DatabaseManager(QObject *parent = nullptr);
  virtual ~DatabaseManager();

  void Open(const QString &fileName);
  void Close();

  QString GetFilename() const;

  void TransactionBegin();
  void TransactionCommit();
  void TransactionRollback();

  DatabaseTablePicture *GetTablePicture();

private:

  QSqlDatabase m_QSqlDatabase;

  DatabaseTablePicture *m_DatabaseTablePicture;

};

#endif // DATABASEMANAGER_H
