#include <numeric>
#include <iostream>
#include <random>

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t c)
{
    uint64_t x = 0;
    uint64_t y = a % c;
    while (b > 0)
    {
        if (b & 1)
        {
            x = (x + y) % c;
        }
        y = (y * 2) % c;
        b /= 2;
    }
    return x % c;
}

uint64_t pow_mod(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t res = 1;
    uint64_t x = a % p;

    while (b)
    {
        if (b & 1)
        {
            res = mul_mod(res, x, p);
        }
        x = mul_mod(x, x, p);
        b /= 2;
    }
    return res % p;
}


bool Miller(uint64_t p, int iteration)
{
    if (p == 1)
    {
        return false;
    }
    if (p != 2 && (p & 1) == 0)
    {
        return false;
    }
    uint64_t s = p - 1;
    while (!(s & 1))
    {
        s /= 2;
    }
    for (int i = 0; i < iteration; ++i)
    {
        uint64_t a = rand() % (p - 1) + 1, temp = s;
        uint64_t mod = pow_mod(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mul_mod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && !(temp & 1))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    uint64_t n;
    std::cin >> n;

    for (int i = 0; i < n; ++i)
    {
        uint64_t k;
        std::cin >> k;
        std::cout << (Miller(k, 10)? "YES" : "NO") << std::endl;
    }

    return 0;
}