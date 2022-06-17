#pragma once

#include "IRacetimingInterface.h"
#include <filesystem>
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
    const std::filesystem::path& path,
    const IRacetimingInterface::RacesTableUpdatedCallback& racesTableUpdatedCallback,
    const IRacetimingInterface::RunnersTableUpdatedCallback& runnersTableUpdatedCallback);
}
