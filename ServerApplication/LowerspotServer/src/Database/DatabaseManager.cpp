
// File includes ---------------------------
#include "DatabaseManager.h"

// Project includes ------------------------
#include "DatabaseTablePicture.h"

// Library includes ------------------------
#include "HelperClasses/Exception.h"

// Qt includes -----------------------------
#include <QString>
#include <QSqlError>
#include <QSqlQuery>

//-----------------------------------------------------------------------------------------------------------------------------

const QString DatabaseManager::_CONST::DATABASE_FILENAME_EXTENSION (".ctb");

const QString DatabaseManager::_CONST::TIMESTAMP_FORMAT ("yyyy-MM-dd hh:mm:ss");

//-----------------------------------------------------------------------------------------------------------------------------

DatabaseManager::DatabaseManager(QObject *parent)
  : QObject(parent)
  , m_DatabaseTablePicture(nullptr)
{
  m_QSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");

  m_DatabaseTablePicture = new DatabaseTablePicture(&m_QSqlDatabase,
                                                    this);
}

//-----------------------------------------------------------------------------------------------------------------------------

DatabaseManager::~DatabaseManager()
{
  delete m_DatabaseTablePicture;
}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseManager::Open(const QString &fileName)
{
  m_QSqlDatabase.setDatabaseName(fileName);

  if (m_QSqlDatabase.open() == false)
  {
    throw Exception(QString("Can't connect to database '%1'; %2").arg(fileName)
                                                                 .arg(m_QSqlDatabase.lastError().text()));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseManager::Close()
{
    m_QSqlDatabase.close();
}

//-----------------------------------------------------------------------------------------------------------------------------

QString DatabaseManager::GetFilename() const
{
  return m_QSqlDatabase.databaseName();
}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseManager::TransactionBegin()
{
  m_QSqlDatabase.transaction();
}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseManager::TransactionCommit()
{
  m_QSqlDatabase.commit();
}

//-----------------------------------------------------------------------------------------------------------------------------

void DatabaseManager::TransactionRollback()
{
  m_QSqlDatabase.rollback();
}

//-----------------------------------------------------------------------------------------------------------------------------

DatabaseTablePicture *DatabaseManager::GetTablePicture()
{
  return m_DatabaseTablePicture;
}
