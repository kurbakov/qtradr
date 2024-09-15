#pragma once

#include <cstdint>
#include <vector>

namespace dsa
{

// return all divisors of X
std::vector<int> divisors(int x)
{
    std::vector<int> res;
    int i = 1;
    while (i * i <= x)
    {
        if (x % i == 0)
        {
            res.push_back(i);
        }
        i++;
    }
    if (i * i == x)
    {
        res.push_back(i);
    }

    return res;
}

// check if X is a prime number
bool is_prime(int64_t x)
{
    if (x == 1)
        return false;

    int64_t i = 2;
    while (i * i <= x)
    {
        if (x % i == 0)
        {
            return false;
        }
        ++i;
    }

    return true;
}

// find all prime numbers from 0 to X
std::vector<int64_t> get_all_primes(int64_t x)
{
    std::vector<bool> sieve(x + 1, true);

    sieve[0] = sieve[1] = false;
    int64_t i = 2;
    while (i * i <= x)
    {
        if (sieve[i])
        {
            int64_t k = i * i;
            while (k <= x)
            {
                sieve[k] = false;
                k += i;
            }
        }
        ++i;
    }

    std::vector<int64_t> res;
    for (int k = 0; k < x; k++)
    {
        if (sieve[k])
        {
            res.push_back(k);
        }
    }

    return res;
}

int64_t gcd(int64_t a, int64_t b)
{
    if (b == 0)
    {
        return a;
    }

    return gcd(b, a % b);
}

int64_t lcm(int64_t a, int64_t b) { return (a * b) / gcd(a, b); }

int max_sub_array_sum(int *arr, int len)
{
    int max_ending = 0;
    int max_slice = 0;
    for (int i = 0; i < len; i++)
    {
        max_ending = std::max(0, max_ending + arr[i]);
        max_slice = std::max(max_slice, max_ending);
    }

    return max_slice;
}

} // namespace dsa