#include <vector>
#include <iostream>
#include <algorithm>

int main()
{
    int n;
    std::cin >> n;

    std::vector<int> numbers(n);
    std::vector<int> answer;
    std::vector<int> previous(n, -1);
    std::vector<int> dp(n, 1);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> numbers[i];
    }
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if(numbers[j] < numbers[i] && dp[i] < dp[j] + 1)
            {
                dp[i] = dp[j] + 1;
                previous[i] = j;
            }
        }   
    }
    
    int max = std::max_element(dp.begin(), dp.end()) - dp.begin();

    for (int i = max; i != -1;)
    {
        answer.push_back(numbers[i]);
        i = previous[i];
    }
    
    std::cout << answer.size() << std::endl;
    std::for_each(answer.crbegin(), answer.crend(), [](int n)
    {
        std::cout << n << ' ';
    });

    return 0;
}