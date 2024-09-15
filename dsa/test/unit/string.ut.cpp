#include "dsa/algo/string.hpp"

#include <gtest/gtest.h>

TEST(String, LongestUniqueSubStr)
{
    std::string msg1{"qwerty"};
    EXPECT_EQ(msg1, dsa::longest_substr_of_unique_chars(msg1));

    std::string msg2{"abcabcbb"};
    EXPECT_EQ("abc", dsa::longest_substr_of_unique_chars(msg2));

    std::string msg3{"bbbbbbbbb"};
    EXPECT_EQ("b", dsa::longest_substr_of_unique_chars(msg3));
}