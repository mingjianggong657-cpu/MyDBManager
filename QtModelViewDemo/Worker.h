#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include <QStringList>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);

public slots:

    // 初始化 Worker 线程自己的数据库连接
    void initDatabase();

    // 查询所有数据库名称
    void loadDatabases();

    // 查询指定数据库中的表
    void loadTables(const QString &database);

    // 查询具体表的数据
    void doWork();

    //查询制定表的数据
    void loadTableData(const QString &database,
                   const QString &table);

    // 执行用户输入的 SQL
    void executeSql(const QString &sql);

signals:

    // 数据库列表查询成功后，把数据库名称传回 GUI
    void databasesLoaded(const QStringList &databases);

    // 表列表查询成功后，把数据库名和表名传回 GUI
    void tablesLoaded(const QString &database,
                      const QStringList &tables);

    // 查询具体表成功后，把表头和数据传回 GUI
    void queryFinished(const QStringList &headers,
                       const QVector<QStringList> &data);

    // 数据库操作失败后，把错误信息传回 GUI
    void queryError(const QString &error);

private:
    // Worker 线程自己的数据库连接
    QSqlDatabase db;
};

#endif // WORKER_H
