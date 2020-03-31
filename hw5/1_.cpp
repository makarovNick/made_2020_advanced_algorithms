#include <iostream>

int main()
{
    int n, x, y;
    std::cin >> n;
    int a[n];
    std::cin >> x >> y >> a[0];

    int m, z, t, b0;
    std::cin >> m;
    int b[2 * m];
    std::cin >> z >> t >> b[0];

    int64_t sums[n];
    sums[0] = a[0];
    for (int i = 1; i < n; ++i)
    {
        a[i] = (x * a[i - 1] + y) & ((1 << 16) - 1);
        sums[i] = sums[i - 1] + a[i];
    }
    for (int i = 1; i < 2 * m; ++i)
    {
        b[i] = (z * b[i - 1] + t) & ((1 << 30) - 1);
        if (b[i] < 0) 
        {
            b[i] = (1 << 30) + b[i];
        }
    }
    int64_t sum = 0;
    for (int i = 0; i < m; ++i)
    {
        int l, r;
        if (b[2 * i] % n > b[2 * i + 1] % n)
        {
            l =  b[2 * i + 1] % n;
            r =  b[2 * i] % n;  
        }
        else
        {
            l =  b[2 * i] % n;
            r =  b[2 * i + 1] % n;
        }
        
        sum += sums[r] - (l == 0 ? 0 : sums[l - 1]);
    }
    
    std::cout << sum;

    return 0;
}