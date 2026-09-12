#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QSqlDatabase>

class Worker : public QObject
{
        Q_OBJECT

	public:
	     explicit Worker(QObject *parent = nullptr);

	public slots:
               void initDatabase();
	       void doWork();

	signals:
	       //查询成功：把表头和数据一起传回GUI
	       void queryFinished(const QStringList &headers,
			          const QVector<QStringList> &data);
	       //查询失败
	       void queryError(const QString &error);

	private:
	       QSqlDatabase db;

};

#endif
