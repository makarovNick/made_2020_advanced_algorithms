#include <algorithm>
#include <iostream>
#include <vector>

struct subTree
{
    int dp;
    int size;
    
    bool is_leaf;
};

subTree DFS(const std::vector<int>& tree, int index)
{
    std::vector<int> children;
    for (int i = 0; i < tree.size(); i++)
    {
        if (tree[i] == index + 1)
        {
            children.push_back(i);
        }
    }

    if (children.size() == 0)
    {
        return subTree{1, 1, true};
    }

    subTree current_node{0,0,false};
    for (auto i : children)
    {
        subTree current_child = DFS(tree, i);

        current_node.size += current_child.size;
        if (current_child.is_leaf == false)
        {
            current_node.dp += current_child.dp;
        }
    }

    return subTree{current_node.size, std::max(current_node.size, 1 + current_node.dp), false};
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> tree(n);

    for (int i = 0; i < n; i++)
    {
        std::cin >> tree[i];
    }

    int root = std::find(tree.begin(), tree.end(), 0) - tree.begin();

    subTree answer = DFS(tree, root);

    std::cout << answer.size;

    return 0;
}