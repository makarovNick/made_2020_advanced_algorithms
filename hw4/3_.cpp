#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#define _CRT_DISABLE_PERFCRIT_LOCKS

#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stdio.h>
//#include <iostream>

inline void write_int(int x)
{
    if (x == -1)
    {
        putchar('-');
        putchar('1');
        return;
    }
    char buf[10], *p = buf;
    do
    {
        *p++ = '0' + x % 10;
        x /= 10;
    }
    while (x);
    do
    {
        putchar(*--p);
    }
    while (p > buf);
}

inline int read_int()
{
    char c;

    while (c = getchar(), c <= ' ');

    int res = c - '0';
    while (c = getchar(), c >= '0' && c <= '9')
    {
        res = res * 10 + (c - '0');
    }

    return res;
}

static inline int fast_mod(int input, int ceil) 
{
    return input < ceil ? input : input % ceil;
}

static inline int gcd(int a, int b)
{
    int c;
    while (b)
    {
        c = b;
        b = fast_mod(a, b);
        a = c;
    }
    return a;
}
 
static inline int mul_mod(int a, int b, int c)
{
    int x = 0;
    int y = fast_mod(a, c);
    while (b > 0)
    {
        if (b & 1)
        {
            x = fast_mod((x + y), c);
        }
        y = fast_mod((y * 2), c);
        b >>= 1;
    }
    return fast_mod(x, c);
}
 
static inline int pow_mod(int a, int b, int p)
{
    int res = 1;
    int x = fast_mod(a, p);
 
    while (b)
    {
        if (b & 1)
        {
            res = mul_mod(res, x, p);
        }
        x = mul_mod(x, x, p);
        b >>= 1;
    }
    return fast_mod(res, p);
}
 
static inline int f(int x, int c, int mod) 
{
    return (mul_mod(x, x, mod) + c) % mod;
}
 
static inline bool Miller(int p, int iteration)
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
 
    int s = p - 1;
    while (!(s & 1))
    {
        s >>= 1;
    }
    for (int i = 0; i < iteration; ++i)
    {
        int a = fast_mod(rand(), (p - 1)) + 1;
        int temp = s;
        int mod = pow_mod(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mul_mod(mod, mod, p);
            temp <<= 1;
        }
        if (mod != p - 1 && !(temp & 1))
        {
            return false;
        }
    }
    return true;
}
 
static inline int rho(int n) 
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
 
    int c = fast_mod(rand(), n - 1) + 1;
    int x = fast_mod(rand(), n - 2) + 2;
    int y = x;
    int factor = 1;
    while (factor == 1)
    {
        x = f(x, c, n);
        y = f(f(y, c, n), c, n);
        factor = gcd(std::abs(x - y), n);
        if (factor == n)
        {
            return rho(n);
        }
    }
 
    return factor;
}
 
static inline std::vector<int> factorize(int n)
{
    std::vector<int> factors;
    while(n != 1)
    {
        int factor = 2;
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
static inline int primitive_root_modulo_n(int n)
{
    if (n == 3)
    {
        return 2;
    }
    auto factors = factorize(n - 1);
 
    for (int i = 2; i <= std::pow(std::log(n), 6); ++i)
    {
        if (pow_mod(i, n - 1, n) == 1)
        {
            for (int j = 0; j < factors.size(); ++j)
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
 
static inline int discrete_log(int g, int h, int mod) 
{
    int m = static_cast<int>(std::ceil(std::sqrt(mod)));
    std::unordered_map<int, int> table;
    int e = 1;
    for (int i = 0; i < m; ++i) 
    {
            table[static_cast<int>(e)] = i;
            e = mul_mod(e, g, mod);
    }
    int factor = pow_mod(g, mod-m-1, mod);
    e = h;
    for (int i = 0; i < m; ++i) 
    {
            if (auto it = table.find(static_cast<int>(e)); it != table.end()) 
            {   
                    return {i*m + it->second};
            }
            e = mul_mod(e, factor, mod);
    }
    return -1;
}
// a = x ^ b mod m
static inline int discrete_root(int a, int b, int m)
{   
    if (a == 1)
    {
        return 1;
    }
    if (b == 1)
    {
        return a;
    }
    int root = primitive_root_modulo_n(m);
    int s = discrete_log(root, a, m);
    auto k = gcd(b , m - 1);
    if (s % k != 0)
    {
        return -1;
    }

    auto res = discrete_log(pow_mod(root, b, m), a, m);
    if (res == -1)      
    {
        return -1;
    }

 
    return pow_mod(root, fast_mod(res, ((m - 1) / k)), m);
}
 
int main()
{
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(0);
    // std::cout.tie(0);   
 
    int n = read_int();
    //std::cin >> n;
    
    for (int i = 0; i < n; ++i)
    {
        int a, b, m;
        //std::cin >> a >> b >> m;
        a = read_int();
        b = read_int();
        m = read_int();
        //std::cout << discrete_root(a, b, m) << '\n';   
        write_int(discrete_root(a, b, m));
        putchar('\n');
    }
    
 
    return 0;
}