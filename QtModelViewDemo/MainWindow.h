#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStringList>

class QLabel;
class QPushButton;
class QTableView;
class QThread;

class Worker;
class TableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();

    void onQueryFinished(const QStringList &headers,
                         const QVector<QStringList> &data);

    void onQueryError(const QString &error);

signals:
    void startWork();

private:
    QLabel *label;
    QPushButton *button;

    QTableView *tableView;
    TableModel *model;

    QThread *thread;
    Worker *worker;
};

#endif // MAINWINDOW_H

