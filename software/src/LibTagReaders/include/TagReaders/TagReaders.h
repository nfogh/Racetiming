#pragma once

#include <memory>
#include "ITagReader.h"

namespace TagReaders {
    std::unique_ptr<ITagReader> CreateM6EReader(std::string_view port);
}
