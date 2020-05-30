#include <algorithm>
#include <iostream>
#include <vector>

struct suffix
{
    int P;
    int C1;
    int C2;
};

std::vector<int> suffix_array(const std::string& txt)
{
    int n = txt.size();

    std::vector<suffix> suffixes(n);

    auto cmp = [](auto& a, auto& b)
    {
        return a.C1 < b.C1 || a.C1 == b.C1 && a.C2 < b.C2;
    };

    for (int i = 0; i < n; ++i)
    {
        suffixes[i].P = i;
        suffixes[i].C1 = txt[i] - 'a';
        suffixes[i].C2 = ((i + 1) < n) ? (txt[i + 1] - 'a') : -1;
    }

    std::sort(suffixes.begin(), suffixes.end(), cmp);

    std::vector<int> pos(n);
    for (int current_length = 4; current_length < 2 * n; current_length <<= 1)
    {
        int current_C = 0;
        int prev_C = suffixes[0].C1;
        suffixes[0].C1 = current_C;
        pos[suffixes[0].P] = 0;

        for (int i = 1; i < n; ++i)
        {
            if (suffixes[i].C1 == prev_C && suffixes[i].C2 == suffixes[i - 1].C2)
            {
                prev_C = suffixes[i].C1;
                suffixes[i].C1 = current_C;
            }
            else
            {
                prev_C = suffixes[i].C1;
                suffixes[i].C1 = ++current_C;
            }
            pos[suffixes[i].P] = i;
        }

        for (int i = 0; i < n; ++i)
        {
            int nextindex = suffixes[i].P + current_length / 2;
            suffixes[i].C2 = (nextindex < n) ? suffixes[pos[nextindex]].C1 : -1;
        }

        std::sort(suffixes.begin(), suffixes.end(), cmp);
    }

    std::vector<int> sa(n);
    for (int i = 0; i < n; ++i)
    {
        sa[i] = suffixes[i].P;
    }

    return sa;
}

int64_t distinct_substrings(std::string& str, std::vector<int>& sa)
{
    int64_t n = sa.size();
    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i) 
    {
        pos[sa[i]] = i;
    }

    str.push_back(1);

    int current = 0;
    int64_t sum = 0;
    for (int i = 0; i < n; ++i) 
    {
        if (current > 0) 
        {
            --current;
        }
        if (pos[i] == n - 1) 
        {
            current = 0;
        } 
        else 
        {
            while (str[i + current] == str[sa[pos[i] + 1] + current]) 
            {
                current++;
            }
            sum += current;
        }
    }
    str.pop_back();

    return n * (n + 1) / 2 - sum;
}

int main()
{
    std::string str;
    std::cin >> str;
    auto sa = suffix_array(str);

    std::cout << distinct_substrings(str, sa);

    return 0;
}