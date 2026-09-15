#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStringList>

class QTableView;
class QTreeWidget;
class QTreeWidgetItem;
class QSplitter;
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

    // Worker 返回数据库列表后，更新左侧树
    void onDatabasesLoaded(const QStringList &databases);

    // 用户点击左侧树节点后执行
    void onTreeItemClicked(QTreeWidgetItem *item,
                           int column);

    // Worker 返回指定数据库的表列表后，更新树
    void onTablesLoaded(const QString &database,
                        const QStringList &tables);

    // Worker 返回查询结果
    void onQueryFinished(const QStringList &headers,
                         const QVector<QStringList> &data);

    // Worker 返回错误信息
    void onQueryError(const QString &error);

signals:

    // 请求 Worker 查询指定数据库中的表
    void requestTables(const QString &database);

private:

    QTreeWidget *treeWidget;
    QTableView *tableView;
    QSplitter *splitter;

    TableModel *model;

    QThread *thread;
    Worker *worker;
};

#endif // MAINWINDOW_H
