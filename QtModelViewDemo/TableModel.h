#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>

class TableModel : public QAbstractTableModel
{
        Q_OBJECT

        public:
           explicit TableModel(QObject *parent = nullptr);
          
	   void setData(const QVector<QStringList> &data);
	   void setHeaders(const QStringList &headers);
          
	   int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	   int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	   QVariant data(const QModelIndex &index,
			   int role = Qt::DisplayRole) const override;

	   QVariant headerData(int section,Qt::Orientation orientation, 
			   int role = Qt::DisplayRole) const override;

	private:
	   QVector<QStringList> m_data;
	   QStringList m_headers;
};

#endif
