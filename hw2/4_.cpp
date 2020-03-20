#include <vector>
#include <iostream>
 
constexpr int MAX_LENGTH = 1000000 * 14;
 
int main()
{
    int n;
    std::cin >> n;
 
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, MAX_LENGTH));
    std::vector<std::vector<int>> adjacency(n, std::vector<int>(n));
    std::vector<std::vector<int>> path(1 << n, std::vector<int>(n ,-1));
 
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cin >> adjacency[i][j];
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        dp[1 << i][i] = 0;
    }
 
    for (int mask = 0; mask < 1 << n; ++mask)
    {
        for (int last = 0; last < n; ++last)
        {
            if (mask && (1 << last) != 0)
            {
                for (int curr = 0; curr < n; ++curr)
                {
                    if (mask && (1 << curr) != 0 && dp[mask][curr] > dp[mask ^ (1 << curr)][last] + adjacency[last][curr])
                    {
                        dp[mask][curr] = dp[mask ^ (1 << curr)][last] + adjacency[last][curr];
                        path[mask][curr] = last;
                    }
                }
            }
        }
    }
 
    int answer = MAX_LENGTH;
    int next = 0;
    for (int i = 0; i < n; ++i)
    {
        if(dp[(1 << n) - 1][i] < answer)
        {
            answer = dp[(1 << n) - 1][i];
            next = i;
        }
    }

    std::cout << answer << std::endl;

    for (int next_mask = (1 << n) - 1; next_mask != 0; next_mask -= (1 << next))
    {
        std::cout << next + 1 << ' ';

        next = path[next_mask + (1 << next)][next];
    }

    return 0;
}