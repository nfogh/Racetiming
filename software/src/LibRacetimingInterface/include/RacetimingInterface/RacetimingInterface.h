#pragma once

#include "IRacetimingInterface.h"
#include <string_view>
#include <memory>

namespace RacetimingInterface {
std::unique_ptr<IRacetimingInterface> CreateWebRacetimingInterface(
    std::string_view endpoint,
    std::string_view apiKey,
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback);
std::unique_ptr<IRacetimingInterface> CreateTestRacetimingInterface(
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback);
std::unique_ptr<IRacetimingInterface> CreateLocalRacetimingInterface(
    const std::string& path,
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback);
}
