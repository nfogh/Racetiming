#include "RacesTableModel.h"

RacesTableModel::RacesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int RacesTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_races.size();
}

int RacesTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant RacesTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case id_col: return QString::number(m_races[index.row()].id);
        case name_col: return QString::fromStdString(m_races[index.row()].name);
        case description_col: return QString::fromStdString(m_races[index.row()].description);
        default:
            throw std::out_of_range("Column index is out of range");
        }
    }

    return QVariant();
}

void RacesTableModel::setData(const RacetimingInterface::RacesTable &races)
{
    const bool isLayoutChanged = races.size() != m_races.size();
    if (isLayoutChanged)
        emit layoutAboutToBeChanged();

    std::vector<QModelIndex> changedIndices;
    for (int row = 0; row < races.size(); row++)
    {
        if (row < m_races.size()) {
            if (m_races[row].id != races[row].id)
                changedIndices.push_back(createIndex(row, 0));
            if (m_races[row].name != races[row].name)
                changedIndices.push_back(createIndex(row, 1));
            if (m_races[row].description != races[row].description)
                changedIndices.push_back(createIndex(row, 2));
        }
    }
    m_races = races;

    if (isLayoutChanged)
        emit layoutChanged();

    for (const auto& index : changedIndices)
        emit dataChanged(index, index, {Qt::DisplayRole});
}

QVariant RacesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case id_col:
            return QString("ID");
        case name_col:
            return QString("name");
        case description_col:
            return QString("description");
        }
    }
    return QVariant();
}
