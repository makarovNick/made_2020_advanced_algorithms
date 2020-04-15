#include <vector>
#include <iostream>
#include <cmath>
#include <set>

const int INF = INFINITY;
const int MAX_N = 200000;
const int MAX_V = 500000;
const int MAX_LOG_V = 20;



struct Vertex
{
    int parent = 0;
    int depth = 1;
    int cost = INF;
    std::set<int> children;
};

void initialize(std::vector<Vertex> & tree, int v = 0, int current = 1)
{
    tree[v].depth = current;
    for (auto i : tree[v].children)
    {
        initialize(tree, i, current + 1);
    }
}

int main()
{
    freopen("minonpath.in", "r", stdin);
    freopen("minonpath.out", "w", stdout);

    std::vector<Vertex> tree(MAX_N);
    std::vector<std::vector<int>> dp(MAX_V, std::vector<int>(MAX_LOG_V));
    std::vector<std::vector<int>> cst(MAX_V, std::vector<int>(MAX_LOG_V));

    int n, m;
    std::cin >> n;
    int max_depth = std::log2(n) + 1;
    for (int i = 1; i < n; ++i)
    {
        std::cin >> tree[i].parent >> tree[i].cost;
        tree[--tree[i].parent].children.insert(i);
    }

    initialize(tree);
    for (int i = 0; i < n; ++i)
    {
        cst[i][0] = tree[i].cost;
        dp[i][0] = tree[i].parent;
        for (int j = 1; j <= max_depth; ++j)
        {
            dp[i][j] = dp[dp[i][j - 1]][j - 1];
            cst[i][j] = std::min(cst[i][j - 1], cst[dp[i][j - 1]][j - 1]);
        }
    }

    std::cin >> m;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        --u;
        --v;
        int res = INF;
        if (tree[v].depth > tree[u].depth)
        {
            std::swap(v, u);
        }
        for (int i = max_depth; i >= 0; --i)
        {
            if (tree[u].depth - tree[v].depth >= (1 << i))
            {
                res = std::min(res, cst[u][i]);
                u = dp[u][i];
            }
        }
        if (v != u)
        {
            for (int i = max_depth; i >= 0; --i)
            {
                if (dp[v][i] != dp[u][i])
                {
                    res = std::min(res, std::min(cst[u][i], cst[v][i]));
                    v = dp[v][i];
                    u = dp[u][i];
                }
            }
            res = std::min(res, std::min(cst[u][0], cst[v][0]));
        }

        std::cout << res << '\n';
    }
    return 0;
}