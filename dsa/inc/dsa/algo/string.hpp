#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dsa
{
std::string longest_substr_of_unique_chars(std::string &msg)
{
    if (msg.length() <= 1)
        return msg;

    std::vector<int> map(256, 0);

    unsigned long start = -1;
    unsigned long end = -1;

    unsigned long idx_left = 0;
    unsigned long idx_right = 0;

    while (idx_right < msg.length() && idx_left <= idx_right)
    {
        while (idx_right < msg.length() && map[msg[idx_right]] == 0)
        {
            map[msg[idx_right]] = 1;
            ++idx_right;
        }

        if ((idx_right - idx_left) > (end - start))
        {
            start = idx_left;
            end = idx_right;
        }

        map[msg[idx_left]] = 0;
        ++idx_left;
    }

    return msg.substr(start, end - start);
}
} // namespace dsa