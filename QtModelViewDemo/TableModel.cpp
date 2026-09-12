#include "TableModel.h"

TableModel::TableModel(QObject *parent)
	  : QAbstractTableModel(parent)
{
}

void TableModel::setData(const QVector<QStringList> &data)
{
           //告诉QTabelView:Model中的数据马上要发生整体变化
	   beginResetModel();

	   //用新的数据替换旧数据
	   m_data = data;

	   //告诉QTableView:数据已经更新完成，请重新读取
	   endResetModel();
}

void TableModel::setHeaders(const QStringList &headers)
{
    beginResetModel();

    m_headers = headers;

    endResetModel();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return m_data.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
       Q_UNUSED(parent);

       if(!m_data.isEmpty()) {
             return m_data[0].size();
       }

       return m_headers.size();
}

QVariant TableModel::data(const QModelIndex &index,int role) const
{
   if(!index.isValid()) 
   {
        return {};
   }

   if(role == Qt::DisplayRole) 
   {
       return m_data[index.row()][index.column()];
   }

   return {};
}

QVariant TableModel::headerData(int section,Qt::Orientation orientation,int role) const
{
   if(role != Qt::DisplayRole)
   {
        return {};
   }

   if(orientation == Qt::Horizontal) 
   {
        if(section >= 0 && section < m_headers.size())
	{
                return m_headers[section];
	}
   }
   
   return {};
}
