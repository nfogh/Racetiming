#ifndef RUNNERSMODEL_H
#define RUNNERSMODEL_H

#include <QAbstractTableModel>

class RunnersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RunnersModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return m_data.size(); };
    int columnCount(const QModelIndex &parent = QModelIndex()) const override { return 2; };
    QVariant headerData(int section, Qt::Orientation, int) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    struct Runner {
        int id;
        int number;
        QString name;
        QString surname;
    };

    void setData(const std::vector<Runner>& data)
    {
        emit layoutAboutToBeChanged();
        m_data = data;
        emit layoutChanged();
        for (int col = 0; col < 3; col++) {
            for (int row = 0; row < m_data.size(); row++) {
                QModelIndex topLeft = createIndex(row, col);
                emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
            }
        }
    };

    const std::vector<Runner>& getData() { return m_data; };

private:
    std::vector<Runner> m_data;
};

#endif // RUNNERSMODEL_H
