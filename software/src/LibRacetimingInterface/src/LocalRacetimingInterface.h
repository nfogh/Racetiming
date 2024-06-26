#pragma once

#include <IRacetimingInterface.h>

namespace RacetimingInterface {
    class LocalRacetimingInterface : public IRacetimingInterface
    {
    public:
        LocalRacetimingInterface(
            const std::string& path,
            const RacesTableUpdatedCallback& racesTableUpdatedCallback,
            const RunnersTableUpdatedCallback& runnersTableUpdatedCallback
        );

        void setRacesTableUpdatedCallback(const RacesTableUpdatedCallback& callback) override { m_racesTableUpdatedCallback = callback; }
        void setRunnersTableUpdatedCallback(const RunnersTableUpdatedCallback& callback) override { m_runnersTableUpdatedCallback = callback; }

        void requestRaces() override;
        void requestRunners(int raceID) override;

        void sendEvent(int numberID, const tm& timestamp, EventType event) override;
        void attachTag(int numberID, std::string_view tag) override;
    private:
        RacesTableUpdatedCallback m_racesTableUpdatedCallback;
        RunnersTableUpdatedCallback m_runnersTableUpdatedCallback;

        std::string m_path;
    };
}
