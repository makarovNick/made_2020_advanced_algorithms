#include <vector>
#include <iostream>

int main()
{
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<uint64_t>> dp(2, std::vector<uint64_t>(1LL << 22));
    std::vector<std::vector<char>> grid(n, std::vector<char>(m));
    dp[0][0] = 1;

    for (int64_t i = 0; i < n; ++i)
    {
        for (int64_t j = 0; j < m; ++j)
        {
            std::cin >> grid[i][j];
        }
    }

    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = m - 1; j >= 0; j--)
        {
            for (uint64_t mask = 0; mask < 1 << m; mask++)
            {
                if (((mask >> j) & 1) || grid[i][j] == 'X')
                {
                    dp[1][mask] = dp[0][mask & (~(1 << j))];
                }
                else
                {
                    int64_t res = 0;
                    if (j + 1 < m && !(mask >> (j + 1) & 1) && (grid[i][j + 1] != 'X'))
                    {
                        res += dp[0][mask | 1 << (j + 1)];
                    }
                    if (i + 1 < n && (grid[i + 1][j] != 'X'))
                    {
                        res += dp[0][mask | 1 << j];
                    }
                    dp[1][mask] = res;
                }
            }
            for (int64_t i = 0; i < 1LL << m; i++)
            { 
                dp[0][i] = dp[1][i];
                dp[1][i] = 0;
            }
        }
    }

    std::cout << dp[0][0];

    return 0;
}