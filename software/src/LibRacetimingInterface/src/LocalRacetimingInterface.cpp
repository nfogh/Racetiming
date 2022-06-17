#include "LocalRacetimingInterface.h"

namespace RacetimingInterface {
    LocalRacetimingInterface::LocalRacetimingInterface(
        const std::filesystem::path& path,
        const RacesTableUpdatedCallback& racesTableUpdatedCallback,
        const RunnersTableUpdatedCallback& runnersTableUpdatedCallback) :
        m_path(path),
        m_racesTableUpdatedCallback(racesTableUpdatedCallback),
        m_runnersTableUpdatedCallback(runnersTableUpdatedCallback)
    {
    }

    void LocalRacetimingInterface::requestRaces()
    {
    }

    void LocalRacetimingInterface::requestRunners(const int raceID)
    {
    }

    void LocalRacetimingInterface::sendEvent(int numberID, const tm& timestamp, EventType event)
    {
    }

    void LocalRacetimingInterface::attachTag(int numberID, std::string_view tag)
    {
    }
}
