#include "LocalRacetimingInterface.h"

namespace RacetimingInterface {
    LocalRacetimingInterface::LocalRacetimingInterface(const std::filesystem::path& path) : m_path(path)
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
