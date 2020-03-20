#include <numeric>
#include <iostream>
#include <algorithm>
#include <vector>
 
constexpr int MIN_COINS = -10001;
 
int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
 
    int N, K;
    std::cin >> N >> K;
 
    std::vector<int> coins(N);
    std::vector<int> dp(N, MIN_COINS);
    std::vector<int> previous(N);
    std::vector<int> answer;
 
    dp[0] = 0;
    previous[0] = -1;
    coins[0] = 0;
    coins[N - 1] = 0;
    for (int i = 1; i < N - 1; ++i)
    {
        std::cin >> coins[i];
    }
 
    for (int i = 1; i < N; ++i) 
    {
        for (int j = 1; j <= K; ++j) 
        {
            int current_index = std::max(i - j, 0);
            if (dp[current_index] + coins[i] > dp[i]) 
            {
                dp[i] = dp[current_index] + coins[i];
                previous[i] = current_index;
            }
        }
    }
 
    std::cout << dp.back() << std::endl;
    for (int i = N - 1; i != -1;)
    {
        answer.push_back(i + 1);
        i = previous[i];
    }
    std::cout << answer.size() - 1 << std::endl;
    std::for_each(answer.crbegin(), answer.crend(), [](int coins)
    {
        std::cout << coins << ' ';
    });
 
    fclose(stdin);
    fclose(stdout);
    return 0;
}