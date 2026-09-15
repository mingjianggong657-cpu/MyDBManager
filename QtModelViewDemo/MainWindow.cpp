#include "MainWindow.h"
#include "Worker.h"
#include "TableModel.h"

#include <QDebug>
#include <QTableView>
#include <QThread>
#include <QSplitter>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      treeWidget(new QTreeWidget(this)),
      tableView(new QTableView(this)),
      splitter(new QSplitter(this)),
      model(new TableModel(this)),
      thread(new QThread(this)),
      worker(new Worker)
{
    // =========================
    // 左侧数据库树
    // =========================

    treeWidget->setHeaderLabel("数据库");

    // =========================
    // 右侧查询结果表格
    // =========================

    tableView->setModel(model);

    // =========================
    // 左右布局
    // =========================

    splitter->addWidget(treeWidget);
    splitter->addWidget(tableView);

    // 右侧比左侧更宽
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    setCentralWidget(splitter);

    resize(800, 500);
    setWindowTitle("MyDBManager");

    // =========================
    // Worker / Thread
    // =========================

    // 将 Worker 的线程归属移动到 worker thread
    worker->moveToThread(thread);

    // Worker线程启动后，在 Worker线程初始化数据库
    connect(thread,
            &QThread::started,
            worker,
            &Worker::initDatabase);

    // Worker查询数据库列表成功
    // → 结果传回GUI线程
    connect(worker,
            &Worker::databasesLoaded,
            this,
            &MainWindow::onDatabasesLoaded);

    // Worker查询表数据成功
    // → 结果传回GUI线程
    connect(worker,
            &Worker::queryFinished,
            this,
            &MainWindow::onQueryFinished);

    // Worker发生错误
    // → 错误信息传回GUI线程
    connect(worker,
            &Worker::queryError,
            this,
            &MainWindow::onQueryError);

    // Worker线程结束后删除Worker
    connect(thread,
            &QThread::finished,
            worker,
            &QObject::deleteLater);

    //树被点击
    connect(treeWidget,
        &QTreeWidget::itemClicked,
        this,
        &MainWindow::onTreeItemClicked);

    //MainWindow请求Worker查询表
    connect(this,
        &MainWindow::requestTables,
        worker,
        &Worker::loadTables);

    //Worker返回表列表
    connect(worker,
        &Worker::tablesLoaded,
        this,
        &MainWindow::onTablesLoaded);

    // 启动Worker线程
    thread->start();
}


MainWindow::~MainWindow()
{
    // 请求Worker线程退出事件循环
    thread->quit();

    // 等待Worker线程真正结束
    thread->wait();
}


void MainWindow::onDatabasesLoaded(
    const QStringList &databases)
{
    // 这个函数运行在GUI线程
    qDebug() << "MainWindow::onDatabasesLoaded thread:"
             << QThread::currentThreadId();

    // 刷新之前先清空旧节点
    treeWidget->clear();

    // 根据数据库列表创建树的顶层节点
    for (const QString &database : databases) {

        QTreeWidgetItem *item =
            new QTreeWidgetItem(treeWidget);

        item->setText(0, database);
    }
}


void MainWindow::onQueryFinished(
    const QStringList &headers,
    const QVector<QStringList> &data)
{
    qDebug() << "MainWindow::onQueryFinished thread:"
             << QThread::currentThreadId();

    // 查询结果已经回到GUI线程
    // 交给Model保存
    model->setHeaders(headers);
    model->setData(data);
}


void MainWindow::onQueryError(const QString &error)
{
    qDebug() << "MainWindow::onQueryError thread:"
             << QThread::currentThreadId();

    qDebug() << "Query error:" << error;
}

void MainWindow::onTreeItemClicked(
    QTreeWidgetItem *item,
    int column)
{
    Q_UNUSED(column);

    // 如果这个节点有父节点，说明它是“表”，不是“数据库”
    if (item->parent() != nullptr) {
        return;
    }

    // 获取用户点击的数据库名称
    QString database = item->text(0);

    qDebug() << "Clicked database:" << database;

    // 通知 Worker 查询该数据库中的表
    emit requestTables(database);
}

void MainWindow::onTablesLoaded(
    const QString &database,
    const QStringList &tables)
{
    // 这个槽运行在 GUI 线程
    qDebug() << "MainWindow::onTablesLoaded thread:"
             << QThread::currentThreadId();

    QTreeWidgetItem *databaseItem = nullptr;

    // 遍历所有顶层节点，找到对应的数据库
    for (int i = 0;
         i < treeWidget->topLevelItemCount();
         ++i) {

        QTreeWidgetItem *item =
            treeWidget->topLevelItem(i);

        if (item->text(0) == database) {
            databaseItem = item;
            break;
        }
    }

    // 如果没找到对应数据库，就不继续处理
    if (databaseItem == nullptr) {
        return;
    }

    // 先删除该数据库下面旧的表节点，防止重复
    databaseItem->takeChildren();

    // 为查询到的每张表创建一个子节点
    for (const QString &table : tables) {

        QTreeWidgetItem *tableItem =
            new QTreeWidgetItem(databaseItem);

        tableItem->setText(0, table);
    }

    // 展开数据库节点，让用户直接看到表
    databaseItem->setExpanded(true);
}
