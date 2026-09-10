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
	       void queryFinished(const QString &result);
	       void queryError(const QString &error);

	private:
	       QSqlDatabase db;

};

#endif
