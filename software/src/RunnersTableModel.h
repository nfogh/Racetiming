#pragma once

#include <QAbstractTableModel>
#include <RacetimingInterface/RacetimingTypes.h>

class RunnersTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    static constexpr int id_col           = 0;
    static constexpr int number_col       = 1;
    static constexpr int name_col         = 2;
    static constexpr int surname_col      = 3;
    static constexpr int numberid_col     = 4;
    static constexpr int tag_col          = 5;
    static constexpr int lapButton_col    = 6;
    static constexpr int finishButton_col = 7;
    static constexpr int attachTag_col    = 8;
    static constexpr int numCols          = 9;

    RunnersTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setData(const RacetimingInterface::RunnersTable& races);

private:
    RacetimingInterface::RunnersTable m_runners;
};
