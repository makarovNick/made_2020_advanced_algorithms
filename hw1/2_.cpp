#include <iostream>
#include <algorithm>
#include <vector>

constexpr int MIN_COINS = -10001;

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N, M;
    std::cin >> N >> M;

    std::vector<std::vector<int>> coins(N, std::vector<int>(M));
    std::vector<std::vector<int>> dp(N, std::vector<int>(M, MIN_COINS));
    std::vector<std::vector<char>> previous(N, std::vector<char>(M));
    std::vector<char> answer;
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            std::cin >> coins[i][j];
        }
    }
    dp[0][0] = coins[0][0];
    previous[0][0] = -1;
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            if(j < M - 1 && dp[i][j] + coins[i][j + 1] > dp[i][j + 1])
            {
                previous[i][j + 1] = 'R';
                dp[i][j + 1] = dp[i][j] + coins[i][j + 1];
            }
            if(i < N - 1 && dp[i][j] + coins[i + 1][j] > dp[i + 1][j])
            {
                previous[i + 1][j] = 'D';
                dp[i + 1][j] = dp[i][j] + coins[i + 1][j];
            }
        }
    }

    std::cout << dp[N - 1][M - 1] << std::endl;
    for (int x = M - 1, y = N - 1; x >= 0 && y >= 0 && previous[y][x] != -1;)
    {
        if(previous[y][x] == 'D')
        {
            y--;
            answer.push_back('D');
        }
        if(previous[y][x] == 'R')
        {
            x--;
            answer.push_back('R');
        }
    }
    std::for_each(answer.crbegin(), answer.crend(), [](char c)
    {
        std::cout << c;
    });
 

    fclose(stdin);
    fclose(stdout);
    return 0;
}