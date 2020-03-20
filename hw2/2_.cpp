#include <iostream>
#include <string>
#include <vector>

constexpr int MAX_LENGTH = 100;

struct DP
{
    int value;
    std::string substring;
};

static inline bool is_good_closing(const std::string& str)
{
    return str.front() == '(' && str.back() == ')' || str.front() == '[' && str.back() == ']' || str.front() == '{' && str.back() == '}';
}

int main()
{
    std::string str;

    std::cin >> str;
    int length = str.length();

    std::vector<std::vector<DP>> dp(length + 1, std::vector<DP>(length + 1));

    for (int i = 0; i < length + 1; ++i)
    {
        dp[i][i] = {1, ""};
    }

    for (int end = 2; end <= length; ++end)
    {
        for (int begin = 0; begin <= length - end; ++begin)
        {
            std::string substr = str.substr(begin, end);

            if (end == 2)
            {
                if (is_good_closing(substr))
                {
                    dp[begin][end + begin - 1] = {0, substr[0] == '(' ? "()" : substr[0] == '[' ? "[]" : "{}"};
                }
                else
                {
                    dp[begin][end + begin - 1] = {2, ""};
                }
            }
            else
            {
                int min = MAX_LENGTH;
                std::string str_min = "";

                if (is_good_closing(substr))
                {
                    if (dp[begin + 1][end + begin - 2].value < min)
                    {
                        min = dp[begin + 1][end + begin - 2].value;

                        if (substr[0] == '(')
                        {
                            str_min = '(' + dp[begin + 1][end + begin - 2].substring + ')';
                        }
                        else if (substr[0] == '[')
                        {
                            str_min = '[' + dp[begin + 1][end + begin - 2].substring + ']';
                        }
                        else if (substr[0] == '{')
                        {
                            str_min = '{' + dp[begin + 1][end + begin - 2].substring + '}';
                        }
                    }
                }
                for (int k = begin; k != end + begin - 1; ++k)
                {
                    if (dp[begin][k].value + dp[k + 1][end + begin - 1].value < min)
                    {
                        min = dp[begin][k].value + dp[k + 1][end + begin - 1].value;
                        str_min = dp[begin][k].substring + dp[k + 1][end + begin - 1].substring;
                    }
                }

                dp[begin][end + begin - 1] = {min, str_min};
            }
        }
    }

    std::cout << dp[0][length - 1].substring;

    return 0;
}