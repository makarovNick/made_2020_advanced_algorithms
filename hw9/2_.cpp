#include <iostream>
#include <vector>

const int INF = 300 * 1e6;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    int n;
    std::cin >> n;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cin >> matrix[i][j];
        }
    }

    std::vector<int> x_delta(n, 0);
    std::vector<int> v_delta(n, 0);
    std::vector<int> zero_indexes(n, -1);

    for (int i = 0; i < n; ++i)
    {
        std::vector<int> links(n, -1);
        std::vector<int> mins(n, INF);
        std::vector<bool> visited(n);

        int ZERO_X = i;
        int ZERO_Y = -1;
        int current_min;
        while (ZERO_X != -1)
        {
            current_min = -1;
            for (int j = 0; j < n; ++j)
            {
                if (!visited[j])
                {
                    if (matrix[ZERO_X][j] - x_delta[ZERO_X] - v_delta[j] < mins[j])
                    {
                        mins[j] = matrix[ZERO_X][j] - x_delta[ZERO_X] - v_delta[j];
                        links[j] = ZERO_Y;
                    }
                    if (current_min == -1 || mins[j] < mins[current_min])
                    {
                        current_min = j;
                    }
                }
            }
            int delta = mins[current_min];
            for (int j = 0; j < n; ++j)
            {
                if (visited[j])
                {
                    x_delta[zero_indexes[j]] += delta;
                    v_delta[j] -= delta;
                }
                else
                {
                    mins[j] -= delta;
                }
            }
            x_delta[i] += delta;

            visited[current_min] = true;
            ZERO_Y = current_min;
            ZERO_X = zero_indexes[current_min];
        }

        for (; links[current_min] != -1; current_min = links[current_min])
        {
            zero_indexes[current_min] = zero_indexes[links[current_min]];
        }
        zero_indexes[current_min] = i;
    }

    int sum = 0;
    std::vector<int> result(n);
    for (int i = 0; i < n; ++i)
    {
        result[i] = zero_indexes[i];
		sum += matrix[result[i]][i];

    }

    std::cout << sum << '\n';
    for (int i = 0; i < n; ++i)
    {
        std::cout << result[i] + 1 << ' ' << i + 1 << '\n';
    }

    return 0;
}