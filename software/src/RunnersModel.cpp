#include "RunnersModel.h"

RunnersModel::RunnersModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant RunnersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return QString("Number");
        case 1: return QString("Name");
        default: throw std::out_of_range("section out of range");
        }
    }
    return QVariant();
}

QVariant RunnersModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return m_data[index.row()].number;
        case 1: return m_data[index.row()].name + " " + m_data[index.row()].surname;
        default: throw std::out_of_range("Column out of range");
        }
    }

    return QVariant();
}
