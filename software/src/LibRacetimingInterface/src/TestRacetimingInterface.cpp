#include "TestRacetimingInterface.h"

namespace RacetimingInterface {
    TestRacetimingInterface::TestRacetimingInterface()
    {
    }

    void TestRacetimingInterface::requestRaces()
    {
        RacesTable races;
        for (int i = 0; i < 4; i++) {
            Race race;
            race.description = "Race";
            race.id = i;
            race.name = "Race " + std::to_string(i + 1);
            races.push_back(race);
        }
        if (m_racesTableUpdatedCallback)
            m_racesTableUpdatedCallback(races);
    }

    void TestRacetimingInterface::requestRunners(const int raceID)
    {
        RunnersTable runners;
        for (int i = 0; i < 10; i++) {
            Runner runner;
            runner.id = i;
            runner.name = "Participant " + std::to_string(i + 1);
            runner.surname = "Surname";
            runner.number = i + 1;
            runner.numberid = i;
            runners.push_back(runner);
        }
        if (m_runnersTableUpdatedCallback)
            m_runnersTableUpdatedCallback(runners);
    }

    void TestRacetimingInterface::sendEvent(int numberID, const tm& timestamp, EventType event)
    {
    }

    void TestRacetimingInterface::attachTag(int numberID, std::string_view tag)
    {
    }
}
