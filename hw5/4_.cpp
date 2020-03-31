
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class sparse_table
{
public:
    sparse_table(const std::vector<int64_t>& data)
        : data(data)
    {
        int64_t pow = 1;
        int64_t max_pow = std::log2(data.size());
        table.resize(max_pow + 1, std::vector<int64_t>(data.size()));
        for (int i = 0; i < data.size(); ++i)
        {
            table[0][i] = data[i];
        }

        for (; pow <= max_pow; ++pow)
        {
            for (int i = 0; i <= data.size() - (1 << pow); ++i)
            {
                table[pow][i] = std::min(table[pow - 1][i], table[pow - 1][i + (1 << (pow - 1))]);
            }
        }
    }
    int64_t rmq(int64_t L, int64_t R) const
    {
        if (R == L)
        {
            return data[R];
        }
        if (L > R)
        {
            std::swap(L, R);
        }
        int64_t pow = std::log2(R - L + 1);
        return std::min(table[pow][L], table[pow][R - (1 << pow) + 1]);
    }

private:
    std::vector<int64_t> data;
    std::vector<std::vector<int64_t> > table;
};

int main()
{

    int64_t n, m;
    std::cin >> n;
    std::vector<int64_t> a(n);
    std::cin >> m >> a[0];

    for (int64_t i = 1; i < n; i++)
    {
        a[i] = (23 * a[i - 1] + 21563) % 16714589;
    }

    sparse_table table(a);

    int64_t u1, v1;
    std::cin >> u1 >> v1;
    int64_t min = table.rmq(u1 - 1, v1 - 1);
    for (int j = 1; j < m; ++j)
    {
        u1 = (17 * u1 + 751 + min + 2 * (j)) % n + 1;
        v1 = (13 * v1 + 593 + min + 5 * (j)) % n + 1;
        min = table.rmq(u1 - 1, v1 - 1);
    }

    std::cout << u1 << ' ' << v1 << ' ' << min;
    return 0;
}