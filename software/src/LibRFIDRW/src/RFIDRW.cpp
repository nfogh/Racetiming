#include "RFIDRW.h"
#include "m6enano.h"

namespace RFIDRW {
    std::unique_ptr<IRFIDRW> CreateM6EReader(const std::string& port)
    {
        return std::make_unique<M6ENano>(port);
    }
}
