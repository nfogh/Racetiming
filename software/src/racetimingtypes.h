#ifndef RACETIMINGTYPES_H
#define RACETIMINGTYPES_H

#include <QString>

namespace RaceTiming {
    struct Runner {
        int id;
        int number;
        int numberid;
        QString name;
        QString surname;
    };

    struct Race {
        int id;
        QString description;
        QString name;
    };

    enum class EventType {
        LapStart = 1,
        Finish = 2
    };
}

#endif // RACETIMINGTYPES_H
