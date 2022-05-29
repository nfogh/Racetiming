#pragma once

#include "IRacetimingInterface.h"
#include <memory>
#include <string_view>

namespace RacetimingInterface {
std::unique_ptr<IRacetimingInterface> CreateWebRacetimingInterface(std::string_view endpoint, std::string_view apiKey);
std::unique_ptr<IRacetimingInterface> CreateTestRacetimingInterface();
std::unique_ptr<IRacetimingInterface> CreateLocalRacetimingInterface();
}
