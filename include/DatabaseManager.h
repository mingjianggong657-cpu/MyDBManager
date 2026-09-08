#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QStringList>

class DatabaseManager
{
 public:
	 DatabaseManager();

	 bool connectToDatabase(const QString& host,const QString& databaseName,const QString& username,const QString& password);

	 void disconnect();

	 bool isOpen() const;

	 QStringList getDatabases();

 private:
	 static const QString CONNECTION_NAME;
};

#endif
