#pragma once

#include <memory>
#include "ITagReader.h"

namespace TagReaders {
    std::unique_ptr<ITagReaderWriter> CreateM6EReader(std::string_view port);
}
