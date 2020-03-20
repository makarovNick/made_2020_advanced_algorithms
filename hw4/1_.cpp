#include <cmath>
#include <vector>
#include <iostream>

std::vector<uint64_t> factorize(uint64_t n)
{
    std::vector<uint64_t> factors;

    if (n == 0)
    {
        return factors;
    }

    if ((n & 1) == 0)
    {
        factors.push_back(2);
        while ((n & 1) == 0)
        {
            n /= 2;
        }
    }
    for (uint64_t i = 3; i <= n * n; i += 2)
    {
        if (n % i == 0)
        {
            factors.push_back(i);
            while (n % i == 0)
            {
                n /= i;
            }
        }
    }
    if (n > 1)
    {
        factors.push_back(n);
    }

    return factors;
}

uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
 
    base = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
        {
            res = (res * base) % mod;
        }
 
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

uint64_t primitive_root_modulo_n(uint64_t n)
{
    if (n == 3)
    {
        return 2;
    }
    auto factors = factorize(n - 1);

    for (uint64_t i = 2; i <= std::pow(std::log(n), 6); ++i)
    {
        if (pow_mod(i, n - 1, n) == 1)
        {
            for (uint64_t j = 0; j < factors.size(); ++j)
            {
                if (pow_mod(i, (n - 1) / factors[j], n) == 1)
                {
                    break;
                }
                if (j == factors.size() - 1)
                {
                    return i;
                }
            }
        }
    }

    return 0;
}

int main()
{
    uint64_t n;
    std::cin >> n;

    std::cout << primitive_root_modulo_n(n);

    return 0;
}