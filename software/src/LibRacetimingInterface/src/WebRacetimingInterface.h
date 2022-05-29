#pragma once

#include <IRacetimingInterface.h>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace RacetimingInterface {
    class WebRacetimingInterface : public IRacetimingInterface, public QObject
    {
    public:
        WebRacetimingInterface(const std::string_view endpoint, const std::string_view apiKey);

        void setRacesTableUpdatedCallback(const RacesTableUpdatedCallback& callback) override { m_racesTableUpdatedCallback = callback; }
        void setRunnersTableUpdatedCallback(const RunnersTableUpdatedCallback& callback) override { m_runnersTableUpdatedCallback = callback; }

        void requestRaces() override;
        void requestRunners(int raceID) override;

        void sendEvent(int numberID, const tm& timestamp, EventType event) override;
        void attachTag(int numberID, std::string_view tag) override;
    private slots:
        void networkAccessManager_finished(QNetworkReply* reply);

    private:
        std::string m_endpoint;
        std::string m_apiKey;

        RacesTableUpdatedCallback m_racesTableUpdatedCallback;
        RunnersTableUpdatedCallback m_runnersTableUpdatedCallback;

        QNetworkAccessManager m_networkAccessManager;
    };
}
