#include <iostream>
#include <vector>
#include <queue>

const int INF = 5000 * 1000;

class Graph
{
    struct Edge
    {
        int to;
        int flow;
        int capacity;
        int from;
    };

public:
    explicit Graph(int n)
        : graph(n)
        , level(n)
    {
    }

    void addEdge(int from, int to, int flow)
    {
        graph[from].push_back({ to, 0, flow, graph[to].size() });
        graph[to].push_back({ from, 0, 0, graph[from].size() - 1 });
    }

    void removeEdge(int from, int to)
    {
        for (auto& i : graph[from])
        {
            if (i.to == to)
            {
                i.flow = 0;
                return;
            }
        }
    }

    bool BFS(int s, int t);
    bool DFS(int s, int t, std::vector<int>& answer, std::vector<bool>& visited);
    int pushFlow(int from, int to, int flow, std::vector<int>& vert_num);
    int maxFlow(int s, int t);

    const std::vector<std::vector<Edge>>& getGraph() const
    {
        return graph;
    }

private:
    std::vector<int> level;
    std::vector<std::vector<Edge>> graph;
};

bool Graph::BFS(int s, int t)
{
    std::fill(level.begin(), level.end(), -1);
    level[s] = 0;

    std::queue<int> queue;
    queue.push(s);

    while (!queue.empty())
    {
        int from = queue.front();
        queue.pop();
        for (auto& i : graph[from])
        {
            if (level[i.to] < 0 && i.flow < i.capacity)
            {
                level[i.to] = level[from] + 1;
                queue.push(i.to);
            }
        }
    }
    return level[t] >= 0;
}

int Graph::pushFlow(int from, int to, int flow, std::vector<int>& vert_num)
{
    if (from == to || flow == 0)
    {
        return flow;
    }

    while (vert_num[from] < graph[from].size())
    {
        Edge& e = graph[from][vert_num[from]];

        if (level[e.to] == level[from] + 1 && e.flow < e.capacity)
        {
            int new_flow = pushFlow(e.to, to, std::min(flow, e.capacity - e.flow), vert_num);
            if (new_flow > 0)
            {
                e.flow += new_flow;
                graph[e.to][e.from].flow -= new_flow;
                return new_flow;
            }
        }
        vert_num[from]++;
    }

    return 0;
}

int Graph::maxFlow(int s, int t)
{
    if (s == t)
    {
        return -1;
    }

    int max_flow = 0;
    while (BFS(s, t))
    {
        std::vector<int> vert_num(graph.size());
        while (int flow = pushFlow(s, t, INF, vert_num))
        {
            max_flow += flow;
        }
    }
    return max_flow;
}

bool Graph::DFS(int s, int t, std::vector<int>& result, std::vector<bool>& visited)
{
    if (!visited[s])
    {
        result.push_back(s);
        visited[s] = true;
        if (s == t)
        {
            return true;
        }
        for (auto& j : graph[s])
        {
            if (!visited[j.to] && j.flow == 1)
            {
                removeEdge(s, j.to);
                if (DFS(j.to, t, result, visited))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int n, m, s, t;
    std::cin >> n >> m >> s >> t;

    Graph g(n);

    for (int i = 0; i < m; ++i)
    {
        int x, y;
        std::cin >> x >> y;
        g.addEdge(x - 1, y - 1, 1);
    }

    std::vector<bool> visited(n);
    std::vector<int> masha;
    std::vector<int> petya;

    if (g.maxFlow(s - 1, t - 1) >= 2)
    {
        std::cout << "YES\n";

        g.DFS(s - 1, t - 1, masha, visited);
        std::fill(visited.begin(), visited.end(), false);
        g.DFS(s - 1, t - 1, petya, visited);

        for (auto i : petya)
        {
            std::cout << i + 1 << " ";
        }

        std::cout << "\n";
        for (auto i : masha)
        {
            std::cout << i + 1 << " ";
        }
    }
    else
    {
        std::cout << "NO\n";
    }

    return 0;
}