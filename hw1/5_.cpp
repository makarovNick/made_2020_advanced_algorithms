#include <iostream>
#include <algorithm>
#include <vector>

constexpr int MAX_COST = 300000;

int main()  
{
    int n;
    std::cin >> n;
    
    if(n == 0) 
    {
        std::cout << 0 << std::endl << 0 << ' ' << 0; 
        return 0;
    }
    if(n == 1)
    {
        int t;
        std::cin >> t;
        if (t > 100)
        {
            std::cout << t << std::endl << 1 << ' ' << 0;
        }
        else
        {
            std::cout << t << std::endl << 0 << ' ' << 0;
        }
        return 0;
    }
    
    std::vector<std::vector<int>> previous(n + 2, std::vector<int>(n + 2));
    std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2));
    std::vector<int> prices(n + 1);
    std::vector<int> answer;

    std::fill(dp[0].begin() + 1, dp[0].end() - 1, MAX_COST);
    for (int i = 0; i < n; i++)
    {
        std::cin >> prices[i + 1];
    }

    dp[0][0] = 0;
    prices[0] = MAX_COST;
    previous[0][0] = 0;
    
    int index = 0;
    int money = 0;
    int used_coupons = 0;
    int not_used_coupons = 0;
    
    for (int i = 1; i < n + 1; ++i)
    {
        if (dp[i - 1][0] + prices[i] < dp[i - 1][1])
        {
            dp[i][0] = dp[i - 1][0] + prices[i];
            previous[i][0] = previous[i - 1][0];
        }
        else
        {
            dp[i][0] = dp[i - 1][1];
            previous[i][0] = previous[i - 1][1] + 1;
        }
        for (int j = 1; j < n + 1; ++j)
        {
            if (j > i)
            {
                dp[i][j] = MAX_COST;
            }
            else if (prices[i] > 100)
            {
                if (dp[i - 1][j - 1] + prices[i] < dp[i - 1][j + 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + prices[i];
                    previous[i][j] = previous[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = dp[i - 1][j + 1];
                    previous[i][j] = previous[i - 1][j + 1] + 1;
                }
            }
            else if (prices[i] <= 100)
            {
                if (dp[i - 1][j] + prices[i] < dp[i - 1][j + 1])
                {
                    dp[i][j] = dp[i - 1][j] + prices[i];
                    previous[i][j] = previous[i - 1][j];
                }
                else
                {
                    dp[i][j] = dp[i - 1][j + 1];
                    previous[i][j] = previous[i - 1][j + 1] + 1;
                }
            }
        }
    }
    money = dp[n][index];
    if (dp[n][1] <= money)
    {
        index = 1;
    }
    not_used_coupons = index;
    used_coupons = previous[n][index];
    for (int i = n; i > 0; --i)
    {
        if (prices[i] != 0)
        {
            if (index == 0)
            {
                if (dp[i][index] - dp[i - 1][index] == prices[i])
                {
                    index = index;
                }
                else if (dp[i - 1][index + 1] == dp[i][index])
                {
                    answer.push_back(i);
                    index++;
                }
            }
            else
            {
                int next_index = -1;
                for (int j = index - 1; j != index + 2; j++)
                {
                    if (dp[i][index] - dp[i - 1][j] == prices[i])
                    {
                        next_index = j;
                        break;
                    }
                }
                if (next_index == -1)
                {
                    for (int j = index - 1; j != index + 2; j++)
                    {
                        if (dp[i][index] == dp[i - 1][j])
                        {
                            next_index = j;
                            answer.push_back(i);
                            break;
                        }
                    }
                }
                index = next_index;
            }
        }
    }

    std::cout << money << std::endl;
    std::cout << not_used_coupons << ' ' << used_coupons << std::endl;
    std::for_each(answer.crbegin(), answer.crend(), [](int n)
    {
        std::cout << n << std::endl;
    });

    return 0;
}