#pragma once

#include <string_view>
#include <ctime>
#include <vector>
#include <functional>

#include "RacetimingTypes.h"

namespace RacetimingInterface {
    class IRacetimingInterface
    {
    public:
        using RacesTableUpdatedCallback = std::function<void(const RacesTable& races)>;
        using RunnersTableUpdatedCallback = std::function<void(const RunnersTable& runners)>;

        virtual void setRacesTableUpdatedCallback(const RacesTableUpdatedCallback& callback) = 0;
        virtual void setRunnersTableUpdatedCallback(const RunnersTableUpdatedCallback& callback) = 0;

        virtual void requestRaces() = 0;
        virtual void requestRunners(int raceID) = 0;

        virtual void sendEvent(int numberID, const tm& timestamp, EventType event) = 0;
        virtual void attachTag(int numberID, std::string_view tag) = 0;
    };
}
