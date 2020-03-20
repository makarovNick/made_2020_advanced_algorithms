#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    int n;
    std::cin >> n;    

    std::vector<std::string> pairs(n);
    std::vector<int> dp(1 << n);
    dp[0] = 0;

    for (int i = 0; i < n; ++i)
    {
        std::cin >> pairs[i];
    }

    for (int i = 1; i < 1 << n; ++i)
    {
        int previous_pairs = i & (~(i - 1));
        // last n zeros
        int new_pairs = __builtin_ctz(previous_pairs);
        dp[i] = dp[i ^ previous_pairs];

        for (int j = new_pairs + 1; j < n; ++j)
        {
            if (pairs[new_pairs][j] == 'Y' && (i & (1 << j)))
            {
                dp[i] = std::max(dp[i], dp[i ^ previous_pairs ^ (1 << j)] + 2);
            }
        }
    }

    std::cout << dp[(1 << n) - 1];

    return 0;
}