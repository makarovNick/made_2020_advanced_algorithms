#include <unordered_map>
#include <cmath>
#include <vector>
#include <iostream>
 
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
 
uint64_t discrete_log(uint64_t g, uint64_t h, uint64_t mod) 
{
    if (g == 0)
    {
        if (h == 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    if (h == 0)
    {
        return 1;
    }
    uint64_t m = static_cast<uint64_t>(std::ceil(std::sqrt(mod)));
    std::unordered_map<uint64_t, uint64_t> table;
    uint64_t e = 1;
    for (uint64_t i = 0; i < m; ++i) 
    {
            table[static_cast<uint64_t>(e)] = i;
            e = mul_mod(e, g, mod);
    }
    uint64_t factor = pow_mod(g, mod-m-1, mod);
    e = h;
    for (uint64_t i = 0; i < m; ++i) 
    {
            if (auto it = table.find(static_cast<uint64_t>(e)); it != table.end()) 
            {
                    return {i*m + it->second};
            }
            e = mul_mod(e, factor, mod);
    }
    return -1;
}
int main()
{
    int64_t a, b, n;
    std::cin >> a >> b >> n;
 
    auto res = discrete_log(a, b, n);
    if (res == (uint64_t) -1)
    {
        std::cout << -1;
    }
    else
    {
        std::cout << res;
    }
     
    return 0;
}