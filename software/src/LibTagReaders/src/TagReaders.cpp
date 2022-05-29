#include <TagReaders.h>
#include "m6enano.h"

namespace TagReaders {
    std::unique_ptr<ITagReader> CreateM6EReader(const std::string_view port)
    {
        return std::make_unique<M6ENano>(port);
    }
}
