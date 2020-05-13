#include <iostream>
#include <climits>
#include <vector>
#include <deque>

size_t n, m;
const int64_t INF = LONG_LONG_MAX;

class Graph
{
    struct Edge
    {
        int64_t from;
        int64_t to;
        int64_t flow;
        int64_t cap;
        int64_t cost;
        int64_t reversed = -1;
    };
    enum state : char
    {
        FAR,
        CLOSE,
        NOT_VISITED,
    };
public:
    Graph(int64_t n)
        : graph(n + 1)
    {
    }

    void addEdge(int64_t from, int64_t to, int64_t flow, int64_t cap, int64_t cost)
    {
        graph[from].push_back({ from, to, flow, cap, cost, static_cast<int64_t>(graph[to].size())});
        graph[to].push_back({ to, from, flow, 0, -cost, static_cast<int64_t>(graph[from].size() - 1)});
    }

    int64_t maxFlowMinCost()
    {
        std::vector<int64_t> d(n + 1);
        std::vector<state> id(n + 1);
        std::vector<Edge*> p(n + 1);

        int64_t min_cost = 0;
        int64_t max_flow = 0;

        while (true)
        {
            std::fill(id.begin(), id.end(), FAR);
            std::fill(d.begin(), d.end(), INF);
            std::deque<int64_t> q;

            d[1] = 0;
            q.push_back(1);

            while (!q.empty())
            {
                int64_t u = q.front();
                q.pop_front();
                id[u] = CLOSE;

                for (int64_t i = 0; i < graph[u].size(); ++i)
                {
                    if (graph[u][i].flow < graph[u][i].cap && d[graph[u][i].to] > d[graph[u][i].from] + graph[u][i].cost)
                    {
                        d[graph[u][i].to] = d[graph[u][i].from] + graph[u][i].cost;

                        if (id[graph[u][i].to] == FAR)
                        {
                            q.push_back(graph[u][i].to);
                        }
                        else if (id[graph[u][i].to] == CLOSE)
                        {
                            q.push_front(graph[u][i].to);
                        }

                        id[graph[u][i].to] = NOT_VISITED;
                        p[graph[u][i].to] = &graph[u][i];
                    }
                }
            }

            int64_t del = INF;

            if (d[n] == INF)
            {
                break;
            }
            else
            {
                for (int64_t u = n; u != 1; u = p[u]->from)
                {
                    Edge* edge = p[u];

                    del = std::min(del, edge->cap - edge->flow);
                }

                for (int64_t u = n; u != 1; u = p[u]->from)
                {
                    Edge* edge = p[u];
                    Edge* reversed = &graph[edge->to][edge->reversed];

                    edge->flow += del;
                    reversed->flow -= del;

                    min_cost += del * edge->cost;
                }

                max_flow += del;
            }
        }

        return min_cost;
    }

private:
    std::vector<std::vector<Edge>> graph;
};

int main()
{
    std::cin >> n >> m;

    Graph graph(n);

    for (int64_t i = 0; i < m; ++i)
    {
        int64_t from, to;
        int64_t cap, cost;
        std::cin >> from >> to >> cap >> cost;

        graph.addEdge(from, to, 0, cap, cost);
    }

    std::cout << graph.maxFlowMinCost();

    return 0;
}