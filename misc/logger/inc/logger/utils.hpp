#pragma once

#include <cstdint>
#include <string_view>

namespace logger
{
consteval size_t placeholders_count(const std::string_view view, const std::string_view pattern)
{
    size_t res{}, pos{};
    while (pos = view.find(pattern, pos), pos != std::string_view::npos)
    {
        res += 1;
        pos += pattern.size();
    }
    return res;
}

template <typename... Args> consteval size_t args_count(Args &&...args) { return sizeof...(args); }

} // namespace logger