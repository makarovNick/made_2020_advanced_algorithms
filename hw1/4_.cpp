#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>

static inline int LD(const std::string& a, const std::string& b) 
{ 
    std::vector<std::vector<int>> dp(2, std::vector<int>(a.size() + 1, 0));
    
    std::iota(dp[0].begin(), dp[0].end(), 0);

    for (int i = 1; i <= b.length(); ++i) 
    { 
        for (int j = 0; j <= a.length(); ++j) 
        {
            if (j == 0) 
            {
                dp[i % 2][j] = i; 
            }
            else if (a[j - 1] == b[i - 1]) 
            { 
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1]; 
            }  
            else 
            { 
                dp[i % 2][j] = 1 + std::min(dp[(i - 1) % 2][j], std::min(dp[i % 2][j - 1], dp[(i - 1) % 2][j - 1])); 
            } 
        } 
    } 
    return dp[b.length() % 2][a.length()];
} 
  

int main()
{
    std::string a, b;
    std::cin >> a >> b;

    std::cout << LD(a, b);

    return 0;
}