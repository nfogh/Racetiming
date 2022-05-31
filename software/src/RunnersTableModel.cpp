#include "RunnersTableModel.h"
#include <QColor>

RunnersTableModel::RunnersTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int RunnersTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_runners.size();
}

int RunnersTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return numCols;
}

template<typename Iterator, typename Delim>
std::string mergeStrings(Iterator begin, const Iterator end, const Delim& d)
{
    return
    begin == end ? "" :
      std::accumulate(
        ++begin, end,
        *begin,
        [](auto& a, auto& b) { return a + "," + b; });
}

QVariant RunnersTableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            const auto& runner = m_runners[index.row()];
            switch (index.column()) {
            case id_col:           return runner.id;
            case number_col:       return runner.number;
            case name_col:         return QString::fromStdString(runner.name);
            case surname_col:      return QString::fromStdString(runner.surname);
            case numberid_col:     return runner.numberid;
/*            case tag_col: {
                QStringList stringList;
                for (const auto& tag : runner.tags)
                    stringList.append(QString::fromStdString(tag));
                return stringList;
            }*/
            case tag_col:          return QString::fromStdString(mergeStrings(runner.tags.cbegin(), runner.tags.cend(), ","));
            case lapButton_col:    return QVariant();
            case finishButton_col: return QVariant();
            case attachTag_col:    return QVariant();
            default:
                throw std::out_of_range("Column index is out of range");
            }
        }
    case Qt::ForegroundRole:
        if (m_selectedRowIndex && *m_selectedRowIndex == index.row())
            return QColor(Qt::green);
    default:
        return QVariant();
    }
}

void RunnersTableModel::setData(const RacetimingInterface::RunnersTable &runners)
{
    const bool isLayoutChanged = runners.size() != m_runners.size();
    if (isLayoutChanged)
        emit layoutAboutToBeChanged();

    std::vector<QModelIndex> changedIndices;
    for (int row = 0; row < runners.size(); row++)
    {
        if (row < m_runners.size()) {
            if (m_runners[row].id != runners[row].id)
                changedIndices.push_back(createIndex(row, id_col));
            if (m_runners[row].number != runners[row].number)
                changedIndices.push_back(createIndex(row, number_col));
            if (m_runners[row].name != runners[row].name)
                changedIndices.push_back(createIndex(row, name_col));
            if (m_runners[row].surname != runners[row].surname)
                changedIndices.push_back(createIndex(row, surname_col));
            if (m_runners[row].numberid != runners[row].numberid)
                changedIndices.push_back(createIndex(row, numberid_col));
            if (m_runners[row].tags != runners[row].tags)
                changedIndices.push_back(createIndex(row, tag_col));
        }
    }
    m_runners = runners;

    if (isLayoutChanged)
        emit layoutChanged();

    for (const auto& index : changedIndices)
        emit dataChanged(index, index, {Qt::DisplayRole});
}

void RunnersTableModel::setSelectedRowIndex(std::optional<int> rowIndex)
{
    const auto prevSelectedRowIndex = m_selectedRowIndex;
    m_selectedRowIndex = rowIndex;
    if (prevSelectedRowIndex && rowIndex && *prevSelectedRowIndex != *rowIndex) {
        emit dataChanged(createIndex(*prevSelectedRowIndex, 0), createIndex(*prevSelectedRowIndex, columnCount() - 1), {Qt::ForegroundRole});
        emit dataChanged(createIndex(*rowIndex, 0), createIndex(*rowIndex, columnCount() - 1), {Qt::ForegroundRole});
    }
}

QVariant RunnersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case id_col:
            return QString("ID");
        case number_col:
            return QString("Number");
        case name_col:
            return QString("Name");
        case surname_col:
            return QString("Surname");
        case numberid_col:
            return QString("Numberid");
        case tag_col:
            return QString("Tag");
        case lapButton_col:
            return QString("Start/Lap");
        case finishButton_col:
            return QString("Finish");
        case attachTag_col:
            return QString("Attach tag");
        default:
            throw std::out_of_range("Column index out of range");
        }
    }
    return QVariant();
}
