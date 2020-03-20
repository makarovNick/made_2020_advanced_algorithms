#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

int main()
{
    int n;
    std::cin >> n;

    std::vector<uint64_t> euler(n + 1);
    std::vector<int> sieve(n + 1);

    uint64_t d = 0;
    uint64_t s0 = 1;
    uint64_t s1 = 1;
    uint64_t phi = 1;

    for (int i = 1; i <= n; ++i) 
    {
        euler[i] = i;
        sieve[i] = i;
    }
    for (int p = 2; p <= n; ++p)
    {
        if (euler[p] == p)
        {
            euler[p] = p - 1;
 
            for (int i = 2 * p; i <= n; i += p)
            {
                if (sieve[i] == i)
                {
                    sieve[i] = p;
                }
                euler[i] = (euler[i] / p) * (p - 1);
            }
        }
        int z = sieve[p];
        int prev = z;
        int count = 0;
        int sum = 1;
        int prod = 1;
        int div_count = 1;
        if (z != p)
        {
            int l = p;  
            while(true)
            {
                if (z == prev)
                {
                    count++;
                }
                else
                {
                    prod *= sum;
                    div_count *= 1 + count;
                    count = 1;
                    sum = 1;
                }
                sum += std::pow(z, count);
                prev = z;
                int u = z;
                z = sieve[l / sieve[z]];
                l = l / u;
                if (z == 1)
                {
                    prod *= sum;
                    div_count *= 1 + count;
                    break;
                }
            }
            sum = prod;
        }
        else
        {
            sum = p + 1;
            div_count = 2;
        }

        d += sieve[p];
        s0 += div_count;
        s1 += sum;
        phi += euler[p];

    }

    std::cout << d << ' ' << s0 << ' ' << s1 << ' ' << phi;

    return 0;
}