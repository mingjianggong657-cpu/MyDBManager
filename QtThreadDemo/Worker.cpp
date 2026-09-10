#include "Worker.h"
#include <QThread>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

Worker::Worker(QObject *parent)
	: QObject(parent)
{
}


void Worker::initDatabase()
{
	qDebug() << "1. initDatabase thread:"
		<< QThread::currentThreadId();

	qDebug() << "2. before addDatabase";

	db = QSqlDatabase::addDatabase("QMYSQL",
			"worker_connection");

	qDebug() << "3. after addDatabase";

	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("webserver");
	db.setUserName("mydb_user");
	db.setPassword("YOUR_PASSWORD");

	qDebug() << "4. before db.open";

	if (!db.open()) {
		QString error = db.lastError().text();

		qDebug() << "5. Database open failed:"
			<<  error;

		emit queryError(error);
		return;
	}

	qDebug() << "6. Database opened successfully.";
}



void Worker::doWork()
{

	qDebug() << "Worker::doWork thread:"
		<< QThread::currentThreadId();

	QSqlQuery query(db);

	if(!query.exec("SELECT DATABASE()")) {
		QString error = query.lastError().text();
		qDebug() << "Query failed:"
			<< query.lastError().text();
		emit queryError(error);
		return;
	}

	if(query.next()) {
		QString result = query.value(0).toString();

		qDebug() << "Current database:" << result;
		emit queryFinished(result);
		return;
	}

	emit queryError("查询没有返回结果");

}


