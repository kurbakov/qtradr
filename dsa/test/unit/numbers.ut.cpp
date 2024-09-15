#include "dsa/algo/numbers.hpp"

#include <gtest/gtest.h>

TEST(Numbers, IsPrime)
{
    // primes
    // source: https://en.wikipedia.org/wiki/List_of_prime_numbers
    EXPECT_TRUE(dsa::is_prime(2));
    EXPECT_TRUE(dsa::is_prime(3));
    EXPECT_TRUE(dsa::is_prime(5));
    EXPECT_TRUE(dsa::is_prime(7));
    EXPECT_TRUE(dsa::is_prime(11));
    EXPECT_TRUE(dsa::is_prime(13));
    EXPECT_TRUE(dsa::is_prime(17));
    EXPECT_TRUE(dsa::is_prime(1087));
    EXPECT_TRUE(dsa::is_prime(1091));
    EXPECT_TRUE(dsa::is_prime(1123));
    EXPECT_TRUE(dsa::is_prime(1571));
    EXPECT_TRUE(dsa::is_prime(7919));

    // not primes
    EXPECT_FALSE(dsa::is_prime(3 + 1));
    EXPECT_FALSE(dsa::is_prime(5 + 1));
    EXPECT_FALSE(dsa::is_prime(7 + 1));
    EXPECT_FALSE(dsa::is_prime(11 + 1));
    EXPECT_FALSE(dsa::is_prime(13 + 1));
    EXPECT_FALSE(dsa::is_prime(17 + 1));
    EXPECT_FALSE(dsa::is_prime(1087 + 1));
    EXPECT_FALSE(dsa::is_prime(1091 + 1));
    EXPECT_FALSE(dsa::is_prime(1123 + 1));
    EXPECT_FALSE(dsa::is_prime(1571 + 1));
    EXPECT_FALSE(dsa::is_prime(7919 + 1));
}

TEST(Numbers, GetAllPrimes)
{
    auto res = dsa::get_all_primes(100);
    std::vector<int64_t> expected{2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37, 41,
                                  43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

    EXPECT_EQ(res.size(), expected.size());
    for (uint64_t i = 0; i < expected.size(); ++i)
    {
        EXPECT_EQ(expected[i], res[i]);
    }
}

TEST(Numbers, LCM) { EXPECT_EQ(60, dsa::lcm(15, 20)); }

TEST(Numbers, GCD) { EXPECT_EQ(5, dsa::gcd(15, 20)); }

TEST(Numbers, MaxSubArraySum)
{
    int arr[] = {5, -7, 3, 5, -2, 4};
    int arr_len = 6;

    EXPECT_EQ(10, dsa::max_sub_array_sum(&arr[0], arr_len));
}