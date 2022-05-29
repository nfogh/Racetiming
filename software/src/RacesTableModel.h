#pragma once
#include <QAbstractTableModel>
#include <RacetimingInterface/RacetimingTypes.h>

class RacesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    constexpr static int id_col = 0;
    constexpr static int name_col = 1;
    constexpr static int description_col = 2;

    RacesTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setData(const RacetimingInterface::RacesTable& races);

private:
    RacetimingInterface::RacesTable m_races;
};
