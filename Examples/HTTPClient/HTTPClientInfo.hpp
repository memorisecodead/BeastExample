#pragma once

#include <string_view>

namespace HTTPClientInfo
{
    namespace HostAddress
    {
        constexpr std::string_view local = "127.0.0.1";
    }
    namespace Port
    {
        constexpr std::uint16_t test = 65003;
    }
}