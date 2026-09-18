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
class QTextEdit;
class QPushButton;
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

    // 用户点击执行按钮
    void onExecuteSql();

    // INSERT / UPDATE / DELETE 执行成功后调用
    void onCommandFinished(int affectedRows);

    

signals:

    // 请求 Worker 查询指定数据库中的表
    void requestTables(const QString &database);

    // 请求 Worker 查询指定表的数据
    void requestTableData(const QString &database,
                          const QString &table);

    // 请求 Worker 执行用户输入的 SQL
    void requestExecuteSql(const QString &sql);

private:

    QTreeWidget *treeWidget;
    QTableView *tableView;
    QSplitter *splitter;

    TableModel *model;
    // SQL 输入框和执行按钮
    QTextEdit *sqlEdit;
    QPushButton *executeButton;
    QThread *thread;
    Worker *worker;
    // 当前正在查看的数据库和表
    QString currentDatabase;
    QString currentTable;
};

#endif // MAINWINDOW_H
