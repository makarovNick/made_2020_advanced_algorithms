#include <numeric>
#include <vector>
#include <iostream>

const constexpr uint64_t MAX_NUMBER = 123123123;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    uint64_t n, a;
    std::cin >> n;

    std::vector<uint64_t> least_prime(MAX_NUMBER + 1);

    std::iota(least_prime.begin(), least_prime.end(), 0);
    
    for (uint64_t i = 2; i <= MAX_NUMBER; ++i)
    {
        if (least_prime[i] == i)
        {
            for (uint64_t j = 2 * i; j <= MAX_NUMBER; j += i)
            {
                if (least_prime[j] == j)
                {
                    least_prime[j] = i;
                }
            }
        }
    }

    for (uint64_t i = 0; i < n; ++i)
    {
        std::cin >> a;
        for (; a != 1; a = a / least_prime[a])
        {
            std::cout << least_prime[a] << ' ';
        }

        std::cout << '\n';
    }
    
    return 0;
}