#include "MainWindow.h"
#include "DatabaseManager.h"

#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) 
	  : QMainWindow(parent),
	    connectButton(new QPushButton("连接并加载数据库",this)),
	    databaseList(new QListWidget(this)),
	    dbManager(new DatabaseManager)
{
         auto *centralWidget = new QWidget(this);
	 auto *layout = new QVBoxLayout(centralWidget);

	 layout->addWidget(connectButton);
	 layout->addWidget(databaseList);

	 setCentralWidget(centralWidget);

	 setWindowTitle("MyDBManager");
	 resize(500,400);

	 connect(connectButton,&QPushButton::clicked,this,&MainWindow::onConnectClicked);
}	

MainWindow::~MainWindow() {
        dbManager->disconnect();
	delete dbManager;
}

void MainWindow::onConnectClicked() {
   bool ok = dbManager->connectToDatabase(
		  "127.0.0.1",
		  "mysql",
		  "mydb_user",
		  "YOUR_PASSWORD"
		  );

  if(!ok) {
       QMessageBox::critical(this,"连接失败","无法连接MySQL");
       return;
  } 

  const QStringList databases = dbManager->getDatabases();

  databaseList->clear();
  databaseList->addItems(databases);

  QMessageBox::information(this,"连接成功","MySQL连接成功           ");

}
