#ifndef RACETIMINGINTERFACE_H
#define RACETIMINGINTERFACE_H

#include <QVector>
#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QStandardItemModel>
#include "racetimingtypes.h"

namespace RaceTiming {
    class RaceTimingInterface : public QObject
    {
        Q_OBJECT
    public:
        RaceTimingInterface(const QUrl& url = QUrl(""), const QString& apiKey = "");

        void setUrl(const QUrl& url) { m_url = "http://" + url.toString(); }
        void setApiKey(const QString& apiKey) { m_apiKey = apiKey; }

        void getRaces();
        void getRunners(int raceID);

        void sendEvent(int numberID, QDateTime timestamp, EventType event);

        QStandardItemModel& GetRacesTable() { return m_racesTable; };
        QStandardItemModel& GetRunnersTable() {return m_runnersTable; };

    private slots:
        void networkAccessManager_finished(QNetworkReply* reply);

    signals:
        void gotRaces(void);
        void gotRunners(void);

    private:
        QNetworkAccessManager m_networkAccessManager;
        QUrl m_url;
        QString m_apiKey;

        QStandardItemModel m_racesTable;
        QStandardItemModel m_runnersTable;
    };
}
#endif // RACETIMINGINTERFACE_H
