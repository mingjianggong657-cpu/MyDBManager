#include "DatabaseManager.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

const QString DatabaseManager::CONNECTION_NAME = "main_connection";

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::connectToDatabase(const QString& host,
		                        const QString& databaseName,
		                        const QString& username,
		                        const QString& password) {

  QSqlDatabase db;
  
  if(QSqlDatabase::contains(CONNECTION_NAME)) {
       db = QSqlDatabase::database(CONNECTION_NAME);
  } else {
       db = QSqlDatabase::addDatabase("QMYSQL",CONNECTION_NAME);
  }

  db.setHostName(host);
  db.setDatabaseName(databaseName);
  db.setUserName(username);
  db.setPassword(password);

  if(!db.open()) {
        qDebug() << "Database connection failed:"
		 << db.lastError().text();
	return false;
  }

  qDebug() << "Database connected successfully";
  return true;
}

void DatabaseManager::disconnect()
{
    if(!QSqlDatabase::contains(CONNECTION_NAME)) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);

    if(db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::isOpen() const {
     if(!QSqlDatabase::contains(CONNECTION_NAME)) {
          return false;
     }
    
     return QSqlDatabase::database(CONNECTION_NAME).isOpen();
}

QStringList DatabaseManager::getDatabases() {
           QStringList databases;

	   if(!isOpen()) {
              return databases;
	   }

	   QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));

	   if(!query.exec("SHOW DATABASES")) {
                qDebug() << "Query failed:"
		       << query.lastError().text();
		return databases;
	   }

	   while(query.next()) {
              databases.append(query.value(0).toString());
	   }

	   return databases;
}
