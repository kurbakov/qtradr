#pragma once

#include <cstdint>
#include <string_view>

namespace logger {
template <typename T>
concept LogStreamer = requires (T stream, std::string_view msg) {
    { stream.write(msg) } -> std::same_as<int64_t>;
    { stream.commit() } -> std::same_as<int64_t>;
};
} // ns logger