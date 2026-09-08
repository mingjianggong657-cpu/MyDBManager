#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListWidget;
class QPushButton;

class DatabaseManager;

class MainWindow : public QMainWindow {
        Q_OBJECT

  public:
       explicit MainWindow(QWidget *parent = nullptr);
       ~MainWindow();
   
  private slots:
       void onConnectClicked();

  private:
       QPushButton *connectButton;
       QListWidget *databaseList;

       DatabaseManager *dbManager;
};

#endif
