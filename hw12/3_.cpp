#include <deque>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Bor
{
    struct node
    {
        node(int previous, char c)
            : previous(previous)
            , value(c)
        {
        }

        int operator[](char c) const
        {
            auto it = links.find(c);
            if (it == links.end())
            {
                return -1;
            }

            return it->second;
        }

        char value;
        int previous;
        int suffix_link = -1;

        std::unordered_map<char, int> links;
        std::unordered_map<char, int> children;
    };
public:
    Bor()
    {
        nodes.emplace_back(-1, '\0');
    }

    void add(const std::string& str)
    {
        int current = 0;
        for (auto c : str)
        {
            int to = nodes[current][c];
            if (to != -1)
            {
                current = to;
                continue;
            }

            nodes[current].links[c] = nodes.size();
            nodes[current].children[c] = nodes.size();
            nodes.emplace_back(current, c);
            current = nodes.size() - 1;
        }
        terminals.push_back(current);
    }

    std::vector<int> countOfOccurance(const std::string& text)
    {
        int current = 0;
        std::unordered_map<int, int> vis;
        for (int i = 0; i < text.size(); ++i)
        {
            current = link(current, text[i]);
            vis[current]++;
        }

        std::deque<int> res;
        res.push_back(0);
        auto z = res.begin();
        while (z != res.end())
        {
            for (auto j : nodes[*z].children)
            {
                res.push_back(j.second);
            }
            z++;
        }
        std::reverse(res.begin(), res.end());

        for (int i = 0; i < res.size(); ++i)
        {
            vis[suffix_link(res[i])] += vis[res[i]];
        }
        std::vector<int> result(terminals.size());
        for (int i = 0; i < terminals.size(); ++i)
        {
            result[i] = vis[terminals[i]];
        }

        return result;
    }

private:
    int suffix_link(int index)
    {
        if (nodes[index].suffix_link != -1)
        {
            return nodes[index].suffix_link;
        }

        if (nodes[index].previous == -1 || nodes[index].previous == 0)
        {
            nodes[index].suffix_link = 0;
        }
        else
        {
            nodes[index].suffix_link = link(suffix_link(nodes[index].previous), nodes[index].value);
        }

        return nodes[index].suffix_link;
    }

    int link(int index, char c)
    {
        int res = nodes[index][c];
        if (res != -1)
        {
            return res;
        }

        if (index == 0)
        {
            nodes[index].links[c] = 0;
        }
        else
        {
            nodes[index].links[c] = link(suffix_link(index), c);
        }

        return nodes[index].links[c];
    }

    std::vector<node> nodes;
    std::vector<int> terminals;
};


int main()
{
    Bor f;
    int n;
    std::cin >> n;
    
    std::string str;
    
    for (int i = 0; i < n; ++i)
    {
        std::cin >> str;
        f.add(str);
    }

    std::cin >> str;

    auto res = f.countOfOccurance(str);

    for (int i = 0; i < n; ++i)
    {
        std::cout << res[i] << '\n';
    }

    return 0;
}