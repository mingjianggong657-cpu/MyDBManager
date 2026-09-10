#include "MainWindow.h"

#include "Worker.h"

#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	label(new QLabel("等待开始...", this)),
	button(new QPushButton("开始任务", this)),
	thread(new QThread(this)),
	worker(new Worker)
{
	qDebug() << "MainWindow constructor begin";
	auto *centralWidget = new QWidget(this);
	auto *layout = new QVBoxLayout(centralWidget);

	layout->addWidget(label);
	layout->addWidget(button);

	setCentralWidget(centralWidget);

	resize(400, 200);
	setWindowTitle("Qt Thread Demo");

	worker->moveToThread(thread);

	connect(thread,
			&QThread::started,
			worker,
			&Worker::initDatabase);
	connect(button,
			&QPushButton::clicked,
			this,
			&MainWindow::onButtonClicked);

	connect(this,
			&MainWindow::startWork,
			worker,
			&Worker::doWork);

	connect(worker,
			&Worker::queryFinished,
			this,
			&MainWindow::onQueryFinished);
	connect(worker,
			&Worker::queryError,
			this,
			&MainWindow::onQueryError);

	connect(thread,
			&QThread::finished,
			worker,
			&QObject::deleteLater);

	thread->start();
	qDebug() << "MainWindow constructor end";
}

MainWindow::~MainWindow()
{
	thread->quit();
	thread->wait();
}

void MainWindow::onButtonClicked()
{
	qDebug() << "MainWindow::onButtonClicked thread:"
		<< QThread::currentThreadId();

	label->setText("任务执行中...");
	emit startWork();

}

void MainWindow::onQueryFinished(const QString &result)
{
	qDebug() << "MainWindow::onWorkFinished thread:"
		<< QThread::currentThreadId();
	label->setText("当前数据库: " + result);
}

void MainWindow::onQueryError(const QString &error)
{
	qDebug() << "MainWindow::onQueryError thread:"
		<< QThread::currentThreadId();

	label->setText("查询失败：" + error);
}
