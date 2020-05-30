#include <iostream>
#include <unordered_map>
#include <vector>

class suffix_tree
{
    struct suffix_node
    {
        int depth;
        int suf_link;
        std::unordered_map<char, int> links;
    };

public:
    suffix_tree(const std::string& str)
    {
        nodes.emplace_back();
        nodes.back().depth = 0;
        nodes.back().suf_link = -1;
        int parent = 0;
        for(int i = 0; i < str.size(); ++i)
        {
            int index = nodes.size();
            nodes.emplace_back();
            nodes.back().depth = nodes[parent].depth + 1;
            while(parent != -1 && nodes[parent].links.find(str[i]) == nodes[parent].links.end())
            {
                nodes[parent].links[str[i]] = index;
                parent = nodes[parent].suf_link;
            }
            if (parent == -1)
            {
                nodes[index].suf_link = 0;
            }
            else
            {
                int transition = nodes[parent].links[str[i]];
                if (nodes[parent].depth + 1 == nodes[transition].depth)
                {
                    nodes[index].suf_link = transition;
                }
                else
                {
                    int copy = nodes.size();
                    nodes.emplace_back();
                    nodes.back().depth = nodes[parent].depth + 1;
                    nodes.back().links = nodes[transition].links;
                    nodes.back().suf_link = nodes[transition].suf_link;
                    while (parent != -1 && nodes[parent].links[str[i]] == transition)
                    {
                        nodes[parent].links[str[i]] = copy;
                        parent = nodes[parent].suf_link;
                    }
                    nodes[transition].suf_link = nodes[index].suf_link = copy;
                }
            }
            parent = index;
        }
    }

    std::vector<suffix_node> nodes;
};

int64_t countDistinctSubstring(suffix_tree& st, int64_t v = 0)
{
    auto current_node = st.nodes[v];
    static std::vector<int64_t> counted(st.nodes.size(), -1);
    if (counted[v] != -1)
    {
        return counted[v];
    }

    int64_t result = 1;
    for (auto i : current_node.links)
    {
        result += countDistinctSubstring(st, i.second);
    }
    counted[v] = result;

    return result;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::string str;
    std::cin >> str;

    suffix_tree st(str);

    std::cout << countDistinctSubstring(st) - 1;

    return 0;
}