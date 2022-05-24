#pragma once

#include <memory>
#include "IRFIDRW.h"

namespace RFIDRW {
    std::unique_ptr<IRFIDRW> CreateM6EReader(const std::string& port);
}
