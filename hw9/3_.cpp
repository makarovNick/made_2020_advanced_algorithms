#include <iostream>
#include <vector>
#include <cstring>

const int INF = 10000;

void make_solution(int i, int j, std::vector<std::vector<int>>& E, const std::vector<std::vector<int>>& path)
{
    int k = path[i][j];
    if (k == i)
    {
        int M = E[0].size();

        if (i < M)
        {
            E[j - M][i]--;
        }
        else
        {
            E[i - M][j]++;
        }
    }
    else
    {
        make_solution(i, k, E, path);
        make_solution(k, j, E, path);
    }
}

int main()
{
    int N, M;
    std::cin >> N >> M;

    int V = N + M;

    std::vector<int> X(N), Y(N), B(N);
    std::vector<int> P(M), Q(M), C(M);
    std::vector<std::vector<int>> E(N, std::vector<int>(M));
    std::vector<std::vector<int>> w(V, std::vector<int>(V, INF));
    std::vector<std::vector<int>> path(V);

    for (int i = 0; i < V; ++i)
    {
        path[i] = std::vector<int>(V, i);
    }

    auto D = [&](int i, int j)
    {
        return std::abs(X[i] - P[j]) + std::abs(Y[i] - Q[j]);
    };



    for (int i = 0; i < N; ++i)
    {
        std::cin >> X[i] >> Y[i] >> B[i];
    }
    for (int i = 0; i < M; ++i)
    {
        std::cin >> P[i] >> Q[i] >> C[i];
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            std::cin >> E[i][j];
            C[j] -= E[i][j];
        }
    }
    for (int j = 0; j < M; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            if (E[i][j] > 0)
            {
                w[j][i + M] = -D(i, j);
            }

            w[i + M][j] = D(i, j);
        }
    }

    for (int k = 0; k < V; ++k)
    {
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                if (w[i][j] > w[i][k] + w[k][j])
                {
                    w[i][j] = w[i][k] + w[k][j];
                    path[i][j] = k;
                    if (w[i][j] < 0 && (i == j || (i < M && j < M && C[j] > 0)))
                    {
                        std::cout << "SUBOPTIMAL" << '\n';
                        make_solution(i, j, E, path);
                        for (int z = 0; z < N; ++z)
                        {
                            for (int x = 0; x < M; ++x)
                            {
                                std::cout << E[z][x] << ' ';
                            }

                            std::cout << '\n';
                        }

                        return 0;
                    }
                }
            }
        }
    }

    std::cout << "OPTIMAL";

    return 0;
}
