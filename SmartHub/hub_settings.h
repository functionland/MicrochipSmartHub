#pragma once
#include <string>

namespace SmartHub {
class HubSettings {
    public:

    bool ParseJson(std::string json);
    std::string DecodeToJson();

    private:
};

}  // namespace SmartHub