#pragma once

#include <string>
#include <vector>

namespace RacetimingInterface {
    struct Runner {
        int id;
        int number;
        int numberid;
        std::vector<std::string> tags; // RFID or barcode or other machine-readable tags
        std::string name;
        std::string surname;
    };
    using RunnersTable = std::vector<Runner>;

    struct Race {
        int id;
        std::string description;
        std::string name;
    };
    using RacesTable = std::vector<Race>;

    enum class EventType {
        LapStart = 1,
        Finish = 2
    };
}
