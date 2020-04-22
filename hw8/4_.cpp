#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

const int INF = 5000 * 1000;

class Graph
{
public:
    struct Edge
    {
        int to;
        int capacity;
        int flow;
        int prev;
    };

    Graph(int n)
        : graph(n)
        , S(0)
        , T(n - 1)
    {
    }

    void addEdge(int from, int to, int flow)
    {
        graph[from].push_back({ to, flow, 0, static_cast<int>(graph[to].size()) });
        graph[to].push_back({ from, 0, 0, static_cast<int>(graph[from].size() - 1) });
    }

    std::vector<int> BFS(int capacity);
    int DFS(int to, std::vector<int>& vert_num, std::vector<int>& level, int flow = INF);
    int maxFlow();

    const std::vector<std::vector<Edge>>& getGraph() const
    {
        return graph;
    }

    int S;
    int T;

private:
    std::vector<std::vector<Edge>> graph;
};


std::vector<int> Graph::BFS(int capacity)
{
    std::vector<int> level(graph.size(), INF);
    level[S] = 0;

    std::queue<int> q;
    q.push(S);

    while (!q.empty())
    {
        int to = q.front();
        q.pop();

        for (auto& e : graph[to])
        {
            if (level[e.to] == INF && e.capacity - e.flow >= capacity)
            {
                level[e.to] = level[to] + 1;
                q.push(e.to);
            }
        }
    }

    return level;
}

int Graph::DFS(int to, std::vector<int>& vert_num, std::vector<int>& level, int flow)
{
    if (to == T || flow == 0)
    {
        return flow;
    }

    for (int i = vert_num[to]; i < graph[to].size(); ++i)
    {
        Edge& e = graph[to][i];

        if (level[e.to] == level[to] + 1)
        {
            int flow_get;
            if (flow_get = DFS(e.to, vert_num, level, std::min(e.capacity - e.flow, flow)))
            {
                e.flow += flow_get;
                Edge& u = graph[to][i];
                graph[u.to][u.prev].flow -= flow_get;

                return flow_get;
            }
            ++vert_num[to];
        }
    }

    return 0;
}

int Graph::maxFlow()
{
    int max_flow = 0;
    for (int cap = INF; cap > 0; cap >>= 1)
    {
        for (auto level = BFS(cap); level[T] != INF; level = BFS(cap))
        {
            std::vector<int> vert_num(graph.size(), 0);

            while (int flow = DFS(S, vert_num, level))
            {
                max_flow += flow;
            }
        }
    }

    return max_flow;
}

void dfs(Graph& g, std::vector<bool>& visited, int to)
{
    if (!visited[to])
    {
        visited[to] = true;

        for (auto& e : g.getGraph()[to])
        {
            if (e.capacity > e.flow)
            {
                dfs(g, visited, e.to);
            }
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    int m, n;
    std::cin >> m >> n;

    std::vector<std::vector<char>> map(m * 2, std::vector<char>(n));
    std::vector<std::pair<int, int>> result;
    std::vector<bool> visited(m * n * 2);
    Graph g(m * n * 2);

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cin >> map[i][j];
            switch (map[i][j])
            {
                case 'A':
                    g.S = n * i + j;
                    break;

                case 'B':
                    g.T = j + (i + m) * n;

                case '.':
                    g.addEdge(j + (i + m) * n, n * i + j, 1);
                    break;

                case '-':
                    g.addEdge(j + (i + m) * n, n * i + j, INF);
                    break;
            }
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (map[i][j] != '#')
            {
                int from = n * i + j;
                int to = j + (i + m) * n;

                if (j + 1 < n && map[i][j + 1] != '#')
                {
                    g.addEdge(from, j + 1 + (i + m) * n, INF);
                    g.addEdge(n * i + j + 1, to, INF);
                }

                if (i + 1 < m && map[i + 1][j] != '#')
                {
                    g.addEdge(from, j + (i + m + 1) * n, INF);
                    g.addEdge(n * (i + 1) + j, to, INF);
                }
            }
        }
    }

    int flow = g.maxFlow();
    dfs(g, visited, g.S);

    for (int i = 0; i < m * n * 2; ++i)
    {
        if (visited[i])
        {
            for (auto& e : g.getGraph()[i])
            {
                if (!visited[e.to] && e.flow == 1)
                {
                    result.push_back({ i, e.to });
                    break;
                }
            }
        }
    }

    if (flow >= INF || flow < 0)
    {
        std::cout << "-1";
    }
    else
    {
        for (auto& i : result)
        {
            map[i.second / n][i.second % n] = '+';
        }

        std::cout << flow << '\n';
        for (int i = 0; i < m; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                std::cout << map[i][j];
            }
            std::cout << '\n';
        }
    }

    return 0;
}