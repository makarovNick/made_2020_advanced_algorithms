#include <iostream>
#include <queue>
#include <vector>

const int INF = 5000 * 1000;

class Graph
{
    struct Edge 
    {
        int to;
        int flow;
        int capacity;
        int prev;
        int number;
    };

public:
    Graph(int n)
        : graph(n)
        , level(n)
    {
    }

    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&&) = delete;
    Graph(const Graph&) = delete;
    Graph(Graph&&) = delete;

    void addEdge(int from, int to, int capacity, int number)
    {
        graph[from].push_back({ to, 0, capacity, static_cast<int>(graph[to].size()), number});
        graph[to].push_back({ from, 0, capacity, static_cast<int>(graph[from].size() - 1), -number});
    }

    bool BFS();
    int pushFlow(int from, int flow, std::vector<int>& vert_num);
    int maxFlow(int s, int t);

    const std::vector<std::vector<Edge>>& getGraph() const
    {
        return graph;
    }

private:
    std::vector<int> level;
    std::vector<std::vector<Edge>> graph;
};

bool Graph::BFS()
{
    std::fill(level.begin(), level.end(), -1);
    level[0] = 0;

    std::queue<int> queue;
    queue.push(0);

    while (!queue.empty())
    {
        int from = queue.front();
        queue.pop();
        for (auto& i : graph[from])
        {
            if (level[i.to] == -1 && i.flow < i.capacity)
            {
                level[i.to] = level[from] + 1;
                queue.push(i.to);
            }
        }
    }
    return level[graph.size() - 1] >= 0;
}

int Graph::pushFlow(int from, int flow, std::vector<int>& vert_num)
{
    if (!flow)
    {
        return 0;
    }

    if (from == graph.size() - 1)
    {
        return flow;
    }

    while (vert_num[from] < graph[from].size())
    {
        Edge& e = graph[from][vert_num[from]];

        if (level[e.to] == level[from] + 1 && e.flow < e.capacity)
        {
            int new_flow = pushFlow(e.to, std::min(flow, e.capacity - e.flow), vert_num);
            if (new_flow > 0)
            {
                e.flow += new_flow;
                graph[e.to][e.prev].flow -= new_flow;

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

    int flows = 0;
    while (BFS())
    {
        std::vector<int> vert_num(t);
        while (int flow = pushFlow(s, INF, vert_num))
        {
            flows += flow;
        }
    }

    return flows;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    int n, m;
    std::cin >> n >> m;

    Graph graph(n);
    for (int i = 0; i < m; ++i)
    {
        int from, to, f;
        std::cin >> from >> to >> f;
        graph.addEdge(from - 1, to - 1, f, i + 1);
    }

    std::cout << graph.maxFlow(0, n - 1) << '\n';

    std::vector<int> flows(m, 0);
    for (auto& i : graph.getGraph())
    {
        for (auto& j : i)
        {
            if (j.number > 0 && j.flow > 0 || j.number < 0 && j.flow < 0)
            {
                flows[std::abs(j.number) - 1] = std::abs(j.flow);
            }
            else
            {
                flows[std::abs(j.number) - 1] = -std::abs(j.flow);
            }
        }
    }

    for (auto i : flows)
    {
        std::cout << i << '\n';
    }

    return 0;
}