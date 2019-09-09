

// File includes ---------------------------
#include "DatabaseTablePicture.h"

// Project includes ------------------------
#include "DatabaseManager.h"

// Library includes ------------------------
#include "HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QDate>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>

//-----------------------------------------------------------------------------------------------------------------------------

const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::TABLE_NAME("Picture");

const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_ID("id");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_USER_ID("user_id");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_SPOT_ID("spot_id");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED("created");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_LATITUDE("latitude");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_LONGITUDE("longitude");
const QString DatabaseTablePicture::_CONST::DATABASE_TABLE::COLUMN_NAME_PUBLISHED("published");

const QStringList DatabaseTablePicture::_CONST::DATABASE_TABLE::ALL_INSERTABLE_COLUMNS = QStringList()
    << COLUMN_NAME_USER_ID
    << COLUMN_NAME_SPOT_ID
    << COLUMN_NAME_CREATED
    << COLUMN_NAME_LATITUDE
    << COLUMN_NAME_LONGITUDE
    << COLUMN_NAME_PUBLISHED;
const QStringList DatabaseTablePicture::_CONST::DATABASE_TABLE::ALL_COLUMNS = QStringList()
    << COLUMN_NAME_ID
    << ALL_INSERTABLE_COLUMNS;

//-----------------------------------------------------------------------------------------------------------------------------

DatabaseTablePicture::DatabaseTablePicture(QSqlDatabase *qSqlDatabase,
                                           QObject *parent)
  : QObject(parent)
  , m_QSqlDatabase(qSqlDatabase)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseTablePicture::InsertRow(const QVariantMap &qVariantMap_Row)
{
  QSqlQuery qSqlQuery(*m_QSqlDatabase);
  qSqlQuery.prepare(QString("INSERT INTO %1 (%2) "
                            "VALUES (:%3)").arg(_CONST::DATABASE_TABLE::TABLE_NAME)
                                           .arg(qVariantMap_Row.keys().join(", "))
                                           .arg(qVariantMap_Row.keys().join(", :")));

  foreach (const QString &key, qVariantMap_Row.keys())
  {
    qSqlQuery.bindValue(QString(":%1").arg(key),
                        qVariantMap_Row.value(key));
  }
  qSqlQuery.exec();
}

//-----------------------------------------------------------------------------------------------------------------------------

int DatabaseTablePicture::GetRowCount() const
{
  QSqlQuery qSqlQuery(QString("SELECT COUNT(*) FROM %1").arg(_CONST::DATABASE_TABLE::TABLE_NAME));

  qSqlQuery.first();
  return qSqlQuery.value(0).toInt();
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<QVariantMap> DatabaseTablePicture::GetRows(const QDate &qDate_From,
                                                 const QDate &qDate_To) const
{
  //  select * from energia where `timestamp` >= '2018-09-02' and `timestamp` <= '2018-09-03'

  QSqlQuery qSqlQuery(*m_QSqlDatabase);
  qSqlQuery.setForwardOnly(true);
  qSqlQuery.prepare(QString("SELECT %1 FROM %2 "
                            "WHERE `%3` >= '%4' "
                            "AND `%3` <= '%5'").arg(_CONST::DATABASE_TABLE::ALL_COLUMNS.join(", "))
                                               .arg(_CONST::DATABASE_TABLE::TABLE_NAME)
                                               .arg(_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED)
                                               .arg(qDate_From.toString(DatabaseManager::_CONST::TIMESTAMP_FORMAT))
                                               .arg(qDate_To.toString(DatabaseManager::_CONST::TIMESTAMP_FORMAT)));
  if(qSqlQuery.exec() == false)
    throw Exception(QString("Can't get database rows; %1").arg(qSqlQuery.lastError().text()));

  QList<QVariantMap> qList_QVariantMap_Rows;
  while (qSqlQuery.next())
  {
    QVariantMap qVariantMap_Row;
    for (int i=0; i < _CONST::DATABASE_TABLE::ALL_COLUMNS.size(); i++)
    {
      qVariantMap_Row.insert(_CONST::DATABASE_TABLE::ALL_COLUMNS.at(i),
                             qSqlQuery.value(i));
    }
    qList_QVariantMap_Rows.append(qVariantMap_Row);
  }

  return qList_QVariantMap_Rows;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariantMap DatabaseTablePicture::GetOldestRow() const
{
  QSqlQuery qSqlQuery(*m_QSqlDatabase);
  qSqlQuery.setForwardOnly(true);
  qSqlQuery.prepare(QString("SELECT %1 FROM %2 "
                            "ORDER BY %3 ASC "
                            "LIMIT 1").arg(_CONST::DATABASE_TABLE::ALL_COLUMNS.join(", "))
                                      .arg(_CONST::DATABASE_TABLE::TABLE_NAME)
                                      .arg(_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED));
  if(qSqlQuery.exec() == false)
    throw Exception(QString("Can't get database rows; %1").arg(qSqlQuery.lastError().text()));

  qSqlQuery.first();
  QVariantMap qVariantMap_Row;
  foreach (const QString &column, _CONST::DATABASE_TABLE::ALL_COLUMNS)
    qVariantMap_Row.insert(column,
                           qSqlQuery.value(column));

  return qVariantMap_Row;
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariantMap DatabaseTablePicture::GetNewestRow() const
{
  QSqlQuery qSqlQuery(*m_QSqlDatabase);
  qSqlQuery.setForwardOnly(true);
  qSqlQuery.prepare(QString("SELECT %1 FROM %2 "
                            "ORDER BY %3 DESC "
                            "LIMIT 1").arg(_CONST::DATABASE_TABLE::ALL_COLUMNS.join(", "))
                                      .arg(_CONST::DATABASE_TABLE::TABLE_NAME)
                                      .arg(_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED));
  if(qSqlQuery.exec() == false)
    throw Exception(QString("Can't get database rows; %1").arg(qSqlQuery.lastError().text()));

  qSqlQuery.first();
  QVariantMap qVariantMap_Row;
  foreach (const QString &column, _CONST::DATABASE_TABLE::ALL_COLUMNS)
    qVariantMap_Row.insert(column,
                           qSqlQuery.value(column));

  return qVariantMap_Row;
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<EntityPicture> DatabaseTablePicture::GetByNewest(const QDateTime &qDateTime_Start,
                                                       int count) const
{
  //  select * from picture where `timestamp` <= '2018-09-03'

  QSqlQuery qSqlQuery(*m_QSqlDatabase);
  qSqlQuery.setForwardOnly(true);
  qSqlQuery.prepare(QString("SELECT %1 FROM %2 "
                            "WHERE `%3` <= '%4' "
                            "ORDER BY %3 DESC "
                            "LIMIT %5").arg(_CONST::DATABASE_TABLE::ALL_COLUMNS.join(", "))
                                       .arg(_CONST::DATABASE_TABLE::TABLE_NAME)
                                       .arg(_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED)
                                       .arg(qDateTime_Start.toString(DatabaseManager::_CONST::TIMESTAMP_FORMAT))
                                       .arg(count));
  if(qSqlQuery.exec() == false)
    throw Exception(QString("Can't get database rows; %1").arg(qSqlQuery.lastError().text()));

  QList<EntityPicture> qList_EntityPicture;
  while (qSqlQuery.next())
  {
    EntityPicture entityPicture;

    entityPicture.Set_Id(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_ID).toLongLong());
    entityPicture.Set_UserId(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_USER_ID).toLongLong());
    entityPicture.Set_SpotId(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_SPOT_ID).toLongLong());
    entityPicture.Set_Created(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_CREATED).toDateTime());
    entityPicture.Set_Latitude(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_LATITUDE).toDouble());
    entityPicture.Set_Longitude(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_LONGITUDE).toDouble());
    entityPicture.Set_Published(qSqlQuery.value(_CONST::DATABASE_TABLE::COLUMN_NAME_PUBLISHED).toBool());

    qList_EntityPicture.append(entityPicture);
  }

  return qList_EntityPicture;
}
