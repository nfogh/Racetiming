#pragma once

#include <QSortFilterProxyModel>
#include <RunnersTableModel.h>
#include <vector>

class ColumnIndexSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ColumnIndexSortFilterProxyModel(QObject *parent = 0, std::vector<int> acceptedColumns = {}) : QSortFilterProxyModel(parent), m_acceptedColumns(acceptedColumns) {};

protected:
    bool filterAcceptsColumn(const int sourceCol, const QModelIndex& sourceParent) const override
    {
        return std::find(m_acceptedColumns.cbegin(), m_acceptedColumns.cend(), sourceCol) != m_acceptedColumns.cend();
    };

    void setAcceptedColumns(const std::vector<int>& acceptedColumns)
    {
        m_acceptedColumns = acceptedColumns;
    };

    const std::vector<int>& acceptedColumns() const
    {
        return m_acceptedColumns;
    }

private:
    std::vector<int> m_acceptedColumns;
};
