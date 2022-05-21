#include "RFIDReaders.h"
#include "m6erfidreader.h"

namespace RFIDReaders {
    std::unique_ptr<IRFIDReader> CreateM6EReader(const std::string& port)
    {
        return std::make_unique<M6ERFIDReader>(port);
    }
}
