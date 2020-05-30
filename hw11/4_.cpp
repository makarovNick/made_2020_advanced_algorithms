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

std::vector<int> lcp_kasai(const std::string& txt, const std::vector<int>& suffixArr)
{
    int n = suffixArr.size();

    std::vector<int> lcp(n);
    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i)
    {
        pos[suffixArr[i]] = i;
    }

    int current_length = 0;
    for (int i = 0; i < n; ++i)
    {
        if (pos[i] == n - 1)
        {
            current_length = 0;
        }
        else
        {
            int j = suffixArr[pos[i] + 1];

            while (i + current_length < n && j + current_length < n && txt[i + current_length] == txt[j + current_length])
            {
                current_length++;
            }

            lcp[pos[i]] = current_length;
            if (current_length > 0)
            {
                current_length--;
            }
        }
    }

    return lcp;
}

std::string lcs(const std::string& a, const std::string& b)
{
    auto T = a + '$' + b;

    auto sa = suffix_array(T);
    auto lcp = lcp_kasai(T, sa);

    int min_len = std::min(a.length(), b.length());
    int start_index;
    int end_index = -1;
    for (int i = 0; i < lcp.size(); ++i)
    {
        if ((sa[i]) < min_len && sa[i + 1] >= min_len 
        ||  (sa[i] >= min_len && sa[i + 1] < min_len))
        {
            if (lcp[i] > end_index) 
            {
                end_index = lcp[i];
                start_index = sa[i];
            }
        }
    }
    
    return T.substr(start_index, end_index);

}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::string a, b;
    std::cin >> a >> b;

    std::cout << lcs(a, b);

    return 0;
}