#pragma once

#include <memory>
#include "IRFIDReader.h"

namespace RFIDReaders {
    std::unique_ptr<IRFIDReader> CreateM6EReader(const std::string& port);
}
