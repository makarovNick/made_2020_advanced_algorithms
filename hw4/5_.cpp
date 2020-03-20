#include <algorithm>
#include <random>
#include <cmath>
#include <vector>
#include <iostream>
 
#define ull_dif_abs(x,y) (((x)>(y))?(x-y):(y-x))

uint64_t fast_mod(uint64_t input, uint64_t ceil) 
{
    return input < ceil ? input : input % ceil;
}
 
uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t c)
{
    uint64_t x = 0;
    uint64_t y = fast_mod(a, c);
    while (b > 0)
    {
        if (b & 1)
        {
            x = fast_mod((x + y), c);
        }
        y = fast_mod((y * 2), c);
        b /= 2;
    }
    return fast_mod(x, c);
}
 
uint64_t pow_mod(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t res = 1;
    uint64_t x = fast_mod(a, p);
 
    while (b)
    {
        if (b & 1)
        {
            res = mul_mod(res, x, p);
        }
        x = mul_mod(x, x, p);
        b /= 2;
    }
    return fast_mod(res, p);
}
 
uint64_t f(uint64_t x, uint64_t c, uint64_t mod) 
{
    return (mul_mod(x, x, mod) + c) % mod;
}
 
bool Miller(uint64_t p, int iteration)
{
    if (p <= 2)
    {
        return true;
    }
    if ((p & 1) == 0)
    {
        return false;
    }
    // std::mt19937_64 gen;
    // gen.seed(std::random_device{}());
 
    uint64_t s = p - 1;
    while (!(s & 1))
    {
        s /= 2;
    }
    for (int i = 0; i < iteration; ++i)
    {
        uint64_t a = fast_mod(rand(), (p - 1)) + 1;
        uint64_t temp = s;
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
 
uint64_t rho(uint64_t n) 
{   
    if (n == 1)
    {
        return 1;
    }
    if (!(n & 1))
    {
        return 2;
    }
 
    // std::mt19937_64 gen;
    // gen.seed(std::random_device{}());
 
    uint64_t c = fast_mod(rand(), n - 1) + 1;
    uint64_t x = fast_mod(rand(), n - 2) + 2;
    uint64_t y = x;
    uint64_t factor = 1;
    while (factor == 1) 
    {
        x = f(x, c, n);
        y = f(f(y, c, n), c, n);
        factor = std::gcd(ull_dif_abs(x, y), n);
        if (factor == n)
        {
            return rho(n);
        }
    }
 
    return factor;
}
 
std::vector<uint64_t> factorize(uint64_t n)
{
    std::vector<uint64_t> factors;
    while(n != 1)
    {
        uint64_t factor = 2;
        if (Miller(n, 10))
        {
            factor = n;
        }
        else
        {
            factor = rho(n);
            while (!Miller(factor, 10))
            {
                factor = rho(factor);
            }
        }
        
        while(!(n % factor))
        {
            n /= factor;
            factors.push_back(factor);
        }
    }
 
    return factors;
}
 
int main()
{
    uint64_t n;
    std::cin >> n;
 
    auto res = factorize(n);
    std::sort(res.begin(), res.end());
    for(uint64_t i : res)
    {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
 
    return 0;
}