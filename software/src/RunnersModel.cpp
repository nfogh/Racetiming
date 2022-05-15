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
        case 3: return m_data[index.row()].id;
        case 0: return m_data[index.row()].number;
        case 2: return m_data[index.row()].numberid;
        case 1: return m_data[index.row()].name;
            + " " + m_data[index.row()].surname;
        default: throw std::out_of_range("Column out of range");
        }
    } else if (role == numberIDRole) {
        return m_data[index.row()].numberid;
    }

    return QVariant();
}
