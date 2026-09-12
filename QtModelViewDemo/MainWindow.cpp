#include "MainWindow.h"
#include "Worker.h"
#include "TableModel.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      label(new QLabel("等待查询...", this)),
      button(new QPushButton("查询数据", this)),
      tableView(new QTableView(this)),
      model(new TableModel(this)),
      thread(new QThread(this)),
      worker(new Worker)
{
    // =========================
    // 创建界面
    // =========================

    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    layout->addWidget(label);
    layout->addWidget(button);
    layout->addWidget(tableView);

    setCentralWidget(centralWidget);

    resize(600, 400);
    setWindowTitle("Qt MySQL Model View Demo");

    // =========================
    // Model / View
    // =========================

    // QTableView 从 Model 中获取数据
    tableView->setModel(model);

    // =========================
    // Worker / Thread
    // =========================

    // 把 Worker 的线程归属改为 worker thread
    worker->moveToThread(thread);

    // 工作线程启动后，在 Worker 线程初始化数据库
    connect(thread,
            &QThread::started,
            worker,
            &Worker::initDatabase);

    // 点击按钮
    connect(button,
            &QPushButton::clicked,
            this,
            &MainWindow::onButtonClicked);

    // MainWindow 发出 startWork
    // Worker 在线程中执行 doWork
    connect(this,
            &MainWindow::startWork,
            worker,
            &Worker::doWork);

    // Worker 查询成功
    // 结果回到 GUI 线程
    connect(worker,
            &Worker::queryFinished,
            this,
            &MainWindow::onQueryFinished);

    // Worker 查询失败
    // 错误信息回到 GUI 线程
    connect(worker,
            &Worker::queryError,
            this,
            &MainWindow::onQueryError);

    // Worker 线程结束后删除 Worker
    connect(thread,
            &QThread::finished,
            worker,
            &QObject::deleteLater);

    // 启动 Worker 线程
    thread->start();
}

MainWindow::~MainWindow()
{
    // 请求 Worker 线程退出
    thread->quit();

    // 等待线程真正结束
    thread->wait();
}

void MainWindow::onButtonClicked()
{
    qDebug() << "MainWindow::onButtonClicked thread:"
             << QThread::currentThreadId();

    label->setText("正在查询...");

    // 通知 Worker 开始数据库查询
    emit startWork();
}

void MainWindow::onQueryFinished(
    const QStringList &headers,
    const QVector<QStringList> &data)
{
    qDebug() << "MainWindow::onQueryFinished thread:"
             << QThread::currentThreadId();

    // Worker 查询完成后，把结果交给 Model
    model->setHeaders(headers);
    model->setData(data);

    label->setText(
        QString("查询成功，共 %1 行").arg(data.size())
    );
}

void MainWindow::onQueryError(const QString &error)
{
    qDebug() << "MainWindow::onQueryError thread:"
             << QThread::currentThreadId();

    label->setText("查询失败：" + error);
}
