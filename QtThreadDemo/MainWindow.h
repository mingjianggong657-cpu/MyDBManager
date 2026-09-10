#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;
class QThread;
class Worker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();
    void onQueryFinished(const QString &result);
    void onQueryError(const QString &error);

signals:
    void startWork();

private:
    QLabel *label;
    QPushButton *button;

    QThread *thread;
    Worker *worker;
};

#endif // MAINWINDOW_H
