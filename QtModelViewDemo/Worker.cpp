#include "Worker.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>

Worker::Worker(QObject *parent)
	: QObject(parent)
{
}

void Worker::initDatabase()
{
	// 当前函数应该运行在 Worker 线程
	qDebug() << "initDatabase thread:"
		<< QThread::currentThreadId();

	// 创建 Worker 线程自己的 MySQL 连接
	db = QSqlDatabase::addDatabase(
			"QMYSQL",
			"worker_connection"
			);

	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("webserver");
	db.setUserName("mydb_user");

	// 本地测试使用自己的密码
	// 注意：不要把真实密码提交到公开 GitHub
	db.setPassword("123456");

	qDebug() << "Before db.open()";

	// 打开数据库
	if (!db.open()) {
		QString error = db.lastError().text();

		qDebug() << "Database open failed:"
			<< error;

		// 错误信息通过信号返回GUI线程
		emit queryError(error);

		return;
	}

	qDebug() << "Database opened successfully.";

	// 数据库连接成功后，立即查询数据库列表
	loadDatabases();
}

void Worker::loadDatabases()
{
	// 确认该函数确实运行在 Worker 线程
	qDebug() << "Worker::loadDatabases thread:"
		<< QThread::currentThreadId();

	// 使用已经初始化好的数据库连接
	QSqlQuery query(db);

	// 查询所有数据库
	if (!query.exec("SHOW DATABASES")) {
		QString error = query.lastError().text();

		qDebug() << "SHOW DATABASES failed:"
			<< error;

		emit queryError(error);

		return;
	}

	// 用 QStringList 保存数据库名称
	QStringList databases;

	// 每调用一次 next()，移动到下一条查询结果
	while (query.next()) {

		// SHOW DATABASES 的数据库名称位于第0列
		QString databaseName =
			query.value(0).toString();

		databases.append(databaseName);
	}

	qDebug() << "Databases:" << databases;

	// 将数据库列表通过Signal发送给GUI线程
	emit databasesLoaded(databases);
}



void Worker::doWork()
{
	qDebug() << "Worker::doWork thread:"
		<< QThread::currentThreadId();

	// 使用 Worker 线程已经建立好的数据库连接
	QSqlQuery query(db);

	if (!query.exec("SELECT * FROM user")) {
		QString error = query.lastError().text();

		qDebug() << "Query failed:"
			<< error;

		emit queryError(error);
		return;
	}

	// ============================
	// 获取列信息
	// ============================

	int columnCount = query.record().count();

	QStringList headers;

	for (int column = 0; column < columnCount; ++column) {
		headers.append(
				query.record().fieldName(column)
			      );
	}

	// ============================
	// 获取数据
	// ============================

	QVector<QStringList> data;

	while (query.next()) {

		QStringList row;

		for (int column = 0;
				column < columnCount;
				++column) {

			row.append(
					query.value(column).toString()
				  );
		}

		data.append(row);
	}

	qDebug() << "Query success.";
	qDebug() << "Rows:" << data.size();
	qDebug() << "Columns:" << headers.size();

	// 把查询结果发送给 GUI 线程
	emit queryFinished(headers, data);
}

void Worker::loadTables(const QString &database)
{
    // 当前函数运行在 Worker 线程，负责查询数据库中的表
    qDebug() << "Worker::loadTables thread:"
             << QThread::currentThreadId();

    QSqlQuery query(db);

    // 切换到用户点击的数据库
    QString sql = QString("USE `%1`").arg(database);

    if (!query.exec(sql)) {
        QString error = query.lastError().text();

        qDebug() << "USE database failed:"
                 << error;

        emit queryError(error);
        return;
    }

    // 查询当前数据库中的所有表
    if (!query.exec("SHOW TABLES")) {
        QString error = query.lastError().text();

        qDebug() << "SHOW TABLES failed:"
                 << error;

        emit queryError(error);
        return;
    }

    QStringList tables;

    // SHOW TABLES 返回的表名位于第 0 列
    while (query.next()) {
        tables.append(query.value(0).toString());
    }

    qDebug() << "Database:" << database;
    qDebug() << "Tables:" << tables;

    // 将数据库名和表名列表传回 GUI 线程
    emit tablesLoaded(database, tables);
}

void Worker::loadTableData(
    const QString &database,
    const QString &table)
{
    // 这个函数应该运行在 Worker 线程
    qDebug() << "Worker::loadTableData thread:"
             << QThread::currentThreadId();

    qDebug() << "Database:" << database
             << "Table:" << table;

    QSqlQuery query(db);

    // 先切换到用户点击的数据库
    QString useSql =
        QString("USE `%1`").arg(database);

    if (!query.exec(useSql)) {
        QString error = query.lastError().text();

        qDebug() << "USE database failed:"
                 << error;

        emit queryError(error);
        return;
    }

    // 查询指定表中的所有数据
    QString selectSql =
        QString("SELECT * FROM `%1`").arg(table);

    if (!query.exec(selectSql)) {
        QString error = query.lastError().text();

        qDebug() << "SELECT failed:"
                 << error;

        emit queryError(error);
        return;
    }

    // 获取查询结果中的列信息
    QSqlRecord record = query.record();

    int columnCount = record.count();

    QStringList headers;

    // 保存列名
    for (int column = 0;
         column < columnCount;
         ++column) {

        headers.append(record.fieldName(column));
    }

    // 保存所有行的数据
    QVector<QStringList> data;

    while (query.next()) {

        QStringList row;

        for (int column = 0;
             column < columnCount;
             ++column) {

            row.append(
                query.value(column).toString()
            );
        }

        data.append(row);
    }

    qDebug() << "Headers:" << headers;
    qDebug() << "Rows:" << data.size();

    // 将查询结果发送回 GUI 线程
    emit queryFinished(headers, data);
}


void Worker::executeSql(const QString &sql)
{
    // 确认 SQL 执行发生在 Worker 线程
    qDebug() << "Worker::executeSql thread:"
             << QThread::currentThreadId();

    qDebug() << "Execute SQL:" << sql;

    QSqlQuery query(db);

    // 执行 SQL
    if (!query.exec(sql)) {
        QString error = query.lastError().text();

        qDebug() << "SQL execute failed:"
                 << error;

        emit queryError(error);
        return;
    }

    // SELECT / SHOW 等查询类 SQL
    if (query.isSelect()) {

        QSqlRecord record = query.record();

        int columnCount = record.count();

        QStringList headers;

        // 获取列名
        for (int column = 0;
             column < columnCount;
             ++column) {

            headers.append(
                record.fieldName(column)
            );
        }

        QVector<QStringList> data;

        // 获取查询结果
        while (query.next()) {

            QStringList row;

            for (int column = 0;
                 column < columnCount;
                 ++column) {

                row.append(
                    query.value(column).toString()
                );
            }

            data.append(row);
        }

        qDebug() << "Headers:" << headers;
        qDebug() << "Rows:" << data.size();

        // 查询结果交给 Model/View
        emit queryFinished(headers, data);
    }
    else {
        // INSERT / UPDATE / DELETE 等非查询 SQL
        int affectedRows = query.numRowsAffected();

        qDebug() << "Command executed successfully.";
        qDebug() << "Affected rows:" << affectedRows;

        // 把影响行数返回 GUI
        emit commandFinished(affectedRows);
    }
}
