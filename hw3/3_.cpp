#include <cmath>
#include <vector>
#include <iostream>
 
uint64_t gcd_extended(uint64_t a, uint64_t b, uint64_t &x, uint64_t &y)  
{  
    if (a == 0)  
    {  
        x = 0;  
        y = 1;
 
        return b;  
    }  
  
    uint64_t x1;   
    uint64_t gcd = gcd_extended(b % a, a, x1, y);  
   
    x = y - (b/a) * x1;  
    y = x1;  
  
    return gcd;  
}  
 
uint64_t mod_inverse(uint64_t a, uint64_t m) 
{
    uint64_t x, y;
    uint64_t res = gcd_extended(a, m, x, y);
 
    return (x + m) % m;
}
 
uint64_t powmod(uint64_t C, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
 
    C = C % mod;
    while (d > 0)
    {
        if (d & 1)
        {
            res = (res * C) % mod;
        }
 
        d = d >> 1;
        C = (C * C) % mod;
    }
    return res;
}
 
 
uint64_t factorize(uint64_t n)
{
    if ((n & 1) == 0)
    {
        return 2;
    }
 
    for (uint64_t i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            return i;
        }
    }
}
 
int main()
{
    uint64_t n, e, C;
    std::cin >> n >> e >> C;
 
    uint64_t p = factorize(n);
 
    std::cout << powmod(C, mod_inverse(e, (p - 1) * ( n / p - 1)), n);
 
    return 0;
}