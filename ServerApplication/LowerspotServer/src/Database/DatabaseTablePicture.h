#ifndef DATABASETABLE_ENERGIA_H
#define DATABASETABLE_ENERGIA_H

// Qt includes --------------------------------------------
#include <QObject>

class QSqlDatabase;

class DatabaseTablePicture : public QObject
{
  Q_OBJECT

public:

  class _CONST
  {
  public:
    class DATABASE_TABLE
    {
    public:

      static const QString TABLE_NAME;

      static const QString COLUMN_NAME_ID;
      static const QString COLUMN_NAME_USER_ID;
      static const QString COLUMN_NAME_SPOT_ID;
      static const QString COLUMN_NAME_CREATED;
      static const QString COLUMN_NAME_LATITUDE;
      static const QString COLUMN_NAME_LONGITUDE;
      static const QString COLUMN_NAME_PUBLISHED;

      static const QStringList ALL_INSERTABLE_COLUMNS;
      static const QStringList ALL_COLUMNS;

    }; // DATABASE_TABLE

  }; // _CONST

  DatabaseTablePicture(QSqlDatabase *qSqlDatabase,
                       QObject *parent = nullptr);


  void InsertRow(const QVariantMap &qVariantMap_Row);

  int GetRowCount() const;
  QList<QVariantMap> GetRows(const QDate &qDate_From,
                             const QDate &qDate_To) const;
  QVariantMap GetOldestRow() const;
  QVariantMap GetNewestRow() const;

private:

  // Link to database
  QSqlDatabase *m_QSqlDatabase;
};

#endif // DATABASETABLE_ENERGIA_H
