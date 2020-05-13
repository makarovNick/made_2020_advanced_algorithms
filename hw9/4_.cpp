#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <map>

const int MAX_N = 10000;
const int INF = 1000 * 1e6;

class Graph
{
    struct Edge
    {
        int to;
        int next;
        int cap;
        int flow;
        int cost;
        int id;
    };
public:
    int addEdge(int u, int v, int cap, int cost, int id)
    {
        graph[index] = {v, head[u], cap, 0, cost, id};
        head[u] = index++;
        graph[index] = {u, head[v], 0, 0, -cost, id};
        head[v] = index++;

        return index;
    }

    bool SPFA(int s, int t)
    {
        std::vector<int> costs(N, INF);
        std::vector<bool> visited(N);
        std::fill(pre.begin(), pre.end(), -1);

        std::queue<int> q;
        costs[s] = 0;
        visited[s] = true;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            visited[u] = false;

            for (int i = head[u]; i != -1; i = graph[i].next)
            {
                int v = graph[i].to;
                if (graph[i].cap > graph[i].flow && costs[v] > costs[u] + graph[i].cost)
                {
                    costs[v] = costs[u] + graph[i].cost;
                    pre[v] = i;
                    if (!visited[v])
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        if (pre[t] == -1)
        {
            return false;
        }

        return true;
    }

    int minCostMaxflow(int s, int t)
    {
        int flow = 0;
        int cost = 0;
        while (SPFA(s, t))
        {
            int Min = INF;
            for (int i = pre[t]; i != -1; i = pre[graph[i ^ 1].to])
            {
                if (Min > graph[i].cap - graph[i].flow)
                    Min = graph[i].cap - graph[i].flow;
            }
            for (int i = pre[t]; i != -1; i = pre[graph[i ^ 1].to])
            {
                graph[i].flow += Min;
                graph[i ^ 1].flow -= Min;
                cost += graph[i].cost * Min;
            }
            flow += Min;
        }

        return flow;
    }

    Graph(int n)
        :   N(MAX_N)
        ,   index(0)
        ,   head(N, -1)
        ,   graph(N)
        ,   pre(N)
    {
    }

    std::vector<Edge> getGraph() const
    {
        return graph;
    }

private:
    int N;
    int index;

    std::vector<Edge> graph;
    std::vector<int> head;
    std::vector<int> pre;
};

struct task
{
    int start, end, cost;
    int id;
};

int main()
{
    int n, k;
    std::cin >> n >> k;

    std::map<int, int> timing;
    std::vector<task> tasks(n);
    std::vector<int> id(n);
    std::vector<int> task_times;

    Graph graph(n);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> tasks[i].start >> tasks[i].end >> tasks[i].cost;
        tasks[i].end += tasks[i].start - 1;
        tasks[i].id = i;
        task_times.push_back(tasks[i].start);
        task_times.push_back(tasks[i].end);
    }

    std::sort(task_times.begin(), task_times.end());
    task_times.erase(std::unique(task_times.begin(), task_times.end()), task_times.end());

    for (int i = 0; i < task_times.size(); ++i)
    {
        timing[task_times[i]] = i + 1;
    }
    
    for (int i = 0; i < task_times.size(); ++i)
    {
        graph.addEdge(i, i + 1, k, 0, 0);
    }
    graph.addEdge(task_times.size(), task_times.size() + 1, k, 0, 0);
    graph.addEdge(task_times.size() + 1, task_times.size() + 2, k, 0, 0);

    for (int i = 0; i < n; ++i)
    {
        id[i] = graph.addEdge(timing[tasks[i].start], timing[tasks[i].end] + 1, 1, -tasks[i].cost, i);
    }

    graph.minCostMaxflow(0, task_times.size() + 2);
    auto res = graph.getGraph();

    for (int i = 0; i < n; ++i)
    {
        std::cout << res[id[i] - 2].flow << ' ';
    }

    return 0;
}