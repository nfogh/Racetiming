#include "RacetimingInterface.h"
#include "WebRacetimingInterface.h"
#include "LocalRacetimingInterface.h"
#include "TestRacetimingInterface.h"

namespace RacetimingInterface {
std::unique_ptr<IRacetimingInterface> CreateWebRacetimingInterface(
    const std::string_view endpoint,
    const std::string_view apiKey,
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback
    )
{
    return std::make_unique<WebRacetimingInterface>(endpoint, apiKey, racesTableUpdatedCallback, runnersTableUpdatedCallback);
}

std::unique_ptr<IRacetimingInterface> CreateLocalRacetimingInterface(
    const std::string& filePath,
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback
    )
{
    return std::make_unique<LocalRacetimingInterface>(filePath, racesTableUpdatedCallback, runnersTableUpdatedCallback);
}

std::unique_ptr<IRacetimingInterface> CreateTestRacetimingInterface(
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback
    )
{
    return std::make_unique<TestRacetimingInterface>(racesTableUpdatedCallback, runnersTableUpdatedCallback);
}
}
