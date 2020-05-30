#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

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

int compare(const std::string& a, const std::string& b)
{
    int min_size = b.size();
    int a_less = 2;
    if (a.size() < min_size)
    {
        min_size = a.size();
        a_less = 0;
    }
    else if (a.size() > min_size)
    {
        a_less = 1;
    }
    
    int res = 2;
    for (int i = 0; i < min_size; ++i)
    {
        if (a[i] < b[i])
        {
            return 0;
        }
        else if (a[i] > b[i])
        {
            return 1;
        }
    }
    
    return a_less;
}

bool search(std::string& pat, std::string& txt, std::vector<int>& suffArr) 
{ 
    int l = 0;
    int r = txt.size()-1;
    while (l <= r) 
    { 
        int mid = l + (r - l)/2; 
        std::string cur = txt.substr(suffArr[mid], pat.size());
        if (cur == pat) 
        { 
            return true; 
        } 

        if (compare(pat, cur) == 0)
        {
            r = mid - 1; 
        }
        else
        {
            l = mid + 1; 
        }
    } 
  
    return false; 
} 


int main ()
{
    int n;
    std::cin >> n;

    std::string t;
    std::vector<std::string> strs(n);
    for (int i = 0; i < n; ++i) 
    {
        std::cin >> strs[i];
    }
    std::cin >> t;
    auto sa = suffix_array(t);

    for (int i = 0; i < n; ++i) 
    {
        std::cout << (search(strs[i], t, sa) ? "YES" : "NO") << '\n';
    }

    return 0;
}