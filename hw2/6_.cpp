#include <iostream>
#include <vector>

constexpr int MAX_OACS = 200;

static inline bool is_chopable(int h1, int h2, int h3)
{
    return h1 < h2 && h3 < h2 || h1 > h2 && h3 > h2;
}

void chop_all(const std::vector<std::vector<int>>& chop_dp, int j, int i, std::vector<int>& answer)
{
    if (chop_dp[j][i] == j)
    {
        return;
    }
    chop_all(chop_dp, j, chop_dp[j][i], answer);
    chop_all(chop_dp, chop_dp[j][i], i, answer);
    answer.push_back(chop_dp[j][i]);
}

void get_answer(const std::vector<std::vector<int>>& chop_dp, const std::vector<std::vector<int>>& last_choped_dp, int j, int i, std::vector<int>& answer)
{
    if (last_choped_dp[j][i] == j)
    {
        chop_all(chop_dp, j, i, answer);
        return;
    }
    get_answer(chop_dp, last_choped_dp, j, last_choped_dp[j][i], answer);
    get_answer(chop_dp, last_choped_dp, last_choped_dp[j][i], i, answer);
}


int main()
{
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> last_choped_dp(n, std::vector<int>(n, -1));
    std::vector<std::vector<int>> chop_dp(n, std::vector<int>(n, -1));
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, MAX_OACS));
    std::vector<int> h(n);
    std::vector<int> answer;

    for (int k = 0; k < n; ++k)
    {
        std::cin >> h[k];
        if(k != n - 1)
        {
            chop_dp[k][k + 1] = k;
        }
    }

    for (int i = 2; i < n; ++i)
    {
        for (int j = 0; j + i < n; ++j)
        {
            for (int k = 1; k < i; ++k)
            {
                if (chop_dp[j][j + k] >= 0 && chop_dp[j + k][j + i] >= 0 && is_chopable(h[j], h[j + k], h[j + i]))
                {
                    chop_dp[j][j + i] = j + k;
                    break;
                }
            }
        }
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j + i < n; j++)
        {
            if (h[j] <= h[j + i])
            {
                if (chop_dp[j][j + i] >= 0)
                {
                    dp[j][j + i] = i - 1;
                    last_choped_dp[j][j + i] = j;
                }

                for (int k = j + 1; k < j + i; k++)
                {
                    if (h[j] <= h[k] && h[k] <= h[j + i] && dp[j][j + i] > dp[j][k] + dp[k][j + i])
                    {
                        dp[j][j + i] = dp[j][k] + dp[k][j + i];
                        last_choped_dp[j][j + i] = k;
                    }
                }
            }
        }
    }

    if (last_choped_dp[0][n - 1] < 0)
    {
        std::cout << -1 << std::endl;
    }
    else
    {
        get_answer(chop_dp, last_choped_dp, 0, n - 1, answer);
        std::cout << answer.size() << std::endl;
        for (int k = 0; k < answer.size(); k++)
        {
            std::cout << answer[k] + 1 << std::endl;
        }
    }

    return 0;
}