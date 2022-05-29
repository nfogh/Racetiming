#include "RacetimingInterface.h"
#include "WebRacetimingInterface.h"
#include "LocalRacetimingInterface.h"
#include "TestRacetimingInterface.h"

namespace RacetimingInterface {
std::unique_ptr<IRacetimingInterface> CreateWebRacetimingInterface(const std::string_view endpoint, const std::string_view apiKey)
{
    return std::make_unique<WebRacetimingInterface>(endpoint, apiKey);
}

std::unique_ptr<IRacetimingInterface> CreateLocalRacetimingInterface(const std::filesystem::path& filePath)
{
    return std::make_unique<LocalRacetimingInterface>(filePath);
}

std::unique_ptr<IRacetimingInterface> CreateTestRacetimingInterface()
{
    return std::make_unique<TestRacetimingInterface>();
}
}
