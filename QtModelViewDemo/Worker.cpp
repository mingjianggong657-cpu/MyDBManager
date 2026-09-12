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
    qDebug() << "initDatabase thread:"
             << QThread::currentThreadId();

    // 创建 Worker 线程自己的数据库连接
    db = QSqlDatabase::addDatabase(
        "QMYSQL",
        "worker_connection"
    );

    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("webserver");
    db.setUserName("mydb_user");

    // 这里只放你自己的本地密码
    db.setPassword("YOUR_PASSWORD");

    qDebug() << "Before db.open()";

    if (!db.open()) {
        QString error = db.lastError().text();

        qDebug() << "Database open failed:"
                 << error;

        emit queryError(error);
        return;
    }

    qDebug() << "Database opened successfully.";
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
