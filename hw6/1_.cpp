#include <vector>
#include <iostream>

class fenwik_tree
{
public:
    fenwik_tree(int64_t _n)
        : array(_n, std::vector<std::vector<int64_t>>(_n, std::vector<int64_t>(_n, 0)))
        , n(_n)
    {
    }

    void update(int64_t x, int64_t y, int64_t z, int64_t delta)
    {
        for (int64_t i = x; i < n; i = i | (i + 1))
        {
            for (int64_t j = y; j < n; j = j | (j + 1))
            {
                for (int64_t k = z; k < n; k = k | (k + 1))
                {
                    array[i][j][k] += delta;
                }
            }
        }
    }

    int64_t sum(int64_t x, int64_t y, int64_t z)
    {
        int64_t result = 0;
        if (x >= 0 && y >= 0 && z >= 0) 
        {
            for (int64_t i = x; i >= 0; i = (i & (i + 1)) - 1)
            {
                for (int64_t j = y; j >= 0; j = (j & (j + 1)) - 1)
                {
                    for (int64_t k = z; k >= 0; k = (k & (k + 1)) - 1)
                    {
                        result += array[i][j][k];
                    }
                }
            }
        }
        return result;
    }

private:
    std::vector<std::vector<std::vector<int64_t>>> array;
    int64_t n;
};

int main()
{
    int64_t n;
    std::cin >> n;
    fenwik_tree tree(n);

    while(std::cin >> n, n != 3)
    {
        if (n == 1)
        {
            int64_t x, y, z, value;
            std::cin >> x >> y >> z >> value;
            tree.update(x, y, z, value);
        }
        else
        {
            int64_t x1, y1, z1, x2, y2, z2;
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            int64_t sum = tree.sum(x2, y2, z2)
                    - tree.sum(x2, y2, z1 -1)
                    - tree.sum(x2, y1 - 1, z2)
                    - tree.sum(x1 - 1, y2, z2)
                    + tree.sum(x1 - 1, y1- 1, z2)
                    + tree.sum(x1 - 1, y2, z1 - 1)
                    + tree.sum(x2, y1 - 1, z1 - 1)
                    - tree.sum(x1 - 1, y1 - 1, z1 - 1);
            
            std::cout << sum << std::endl;
        }
    }   

    return 0;
}