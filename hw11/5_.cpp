#include <iostream>
#include <vector>
#include <string>

std::string kth_cycle(std::string& str, int K)
{
    int n = str.size();
    std::vector<int> suffix_array(n);
    std::vector<int> classes(n);
    std::vector<int> alphabet_count(100, 0);
    std::vector<int> P(n);
    std::vector<int> C(n);

    for (int i = 0; i < n; ++i)
    {
        ++alphabet_count[str[i] - 32];
    }
    for (int i = 1; i < 100; ++i)
    {
        alphabet_count[i] += alphabet_count[i - 1];
    }
    for (int i = 0; i < n; ++i)
    {
        suffix_array[--alphabet_count[str[i] - 32]] = i;
    }

    int current_class = 1;
    classes[suffix_array[0]] = 0;
    for (int i = 1; i < n; ++i)
    {
        if (str[suffix_array[i - 1]] != str[suffix_array[i]])
        {
            ++current_class;
        }
        classes[suffix_array[i]] = current_class - 1;
    }
    // find previous class
    for (int k = 0; (1 << k) < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            P[i] = suffix_array[i] - (1 << k);
            if (P[i] < 0)
            {
                P[i] += n;
            }
        }
        std::vector<int> counter(current_class, 0);
        for (int i = 0; i < P.size(); ++i)
        {
            ++counter[classes[P[i]]];
        }
        for (int i = 1; i < counter.size(); ++i)
        {
            counter[i] += counter[i - 1];
        }
        for (int i = P.size() - 1; i >= 0; --i)
        {
            suffix_array[--counter[classes[P[i]]]] = P[i];
        }

        current_class = 1;
        C[suffix_array[0]] = 0;
        for (int i = 1; i < n; ++i)
        {
            int C1_index = (suffix_array[i] + (1 << k)) % n;
            int C2_index = (suffix_array[i - 1] + (1 << k)) % n;
            if (classes[suffix_array[i]] != classes[suffix_array[i - 1]]
                || classes[C1_index] != classes[C2_index])
            {
                ++current_class;
            }
            C[suffix_array[i]] = current_class - 1;
        }

        for (int i = 0; i < classes.size(); ++i)
        {
            classes[i] = C[i];
        }
    }

    std::string res;
    for (int i = 0; i < n; ++i)
    {
        if (C[suffix_array[i]] == K - 1)
        {
            if (suffix_array[i] < str.size())
            {
                res = (str.substr(suffix_array[i]) 
                    + str.substr(0, suffix_array[i]));
            }
            else
            {
                res = (str.substr(suffix_array[i] - str.size())
                    + str.substr(0, suffix_array[i] - str.size()));
            }
            return res;
        }
    }

    return "IMPOSSIBLE";
}

int main()
{
    std::string str;
    int K;
    std::cin >> str >> K;

    std::cout << kth_cycle(str, K);

    return 0;
}