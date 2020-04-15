#include <iostream>
#include <vector>
#include <cmath>
#include <set>

const int MAX_N = 200000;
const int MAX_V = 500000;
const int MAX_LOG_V = 20;

struct Vertex
{
    int parent = 0;
    int depth = 1;
    std::set<int> children;
};

void initialize(std::vector<Vertex>& tree, int v = 0, int depth = 1)
{
    tree[v].depth = depth;
    for (auto i : tree[v].children)
    {
        initialize(tree, i, depth + 1);
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    int n, m;
    std::cin >> n;
    std::vector<Vertex> tree(MAX_N);
    std::vector<std::vector<int>> dp(MAX_V, std::vector<int>(MAX_LOG_V));
    int max_depth = std::log2(n) + 1;
    for (int i = 1; i < n; ++i)
    {
        std::cin >> tree[i].parent;
        tree[--tree[i].parent].children.insert(i);
    }

    initialize(tree);
    for (int i = 0; i < n; ++i)
    {
        dp[i][0] = tree[i].parent;
        for (int j = 1; j <= max_depth; ++j)
        {
            dp[i][j] = dp[dp[i][j - 1]][j - 1];
        }
    }
    std::cin >> m;

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        u--;
        v--;
        int res = v;
        if (tree[v].depth > tree[u].depth)
        {
            res = u;
            std::swap(v, u);
        }
        for (int i = max_depth; i >= 0; --i)
        {
            if (tree[u].depth - tree[v].depth >= (1 << i))
            {
                u = dp[u][i];
            }
        }
        if (v != u)
        {
            for (int i = max_depth; i >= 0; --i)
            {
                if (dp[v][i] != dp[u][i])
                {
                    v = dp[v][i];
                    u = dp[u][i];
                }
            }
            res = tree[v].parent;  
        }

        std::cout << res + 1 << '\n';
    }

    return 0;   
}