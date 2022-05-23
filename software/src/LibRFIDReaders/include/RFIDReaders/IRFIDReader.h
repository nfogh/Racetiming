#pragma once

#include <string>
#include <functional>

namespace RFIDReaders {
    class IRFIDReader
    {
    public:
        using TagDetectedCallbackFunc = std::function<void(const std::string& epc)>;
        using ConnectedCallbackFunc = std::function<void()>;
        using DisconnectedCallbackFunc = std::function<void()>;

        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void setTagDetectedCallback(const TagDetectedCallbackFunc& callback) = 0;
        virtual void setConnectedCallback(const ConnectedCallbackFunc& callback) = 0;
        virtual void setDisconnectedCallback(const DisconnectedCallbackFunc& callback) = 0;
    };
}
