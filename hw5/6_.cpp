#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

const int INF = 2147483647;

struct query
{
    int i, j, q;
};

class segment_tree
{
public:
    explicit segment_tree(const std::vector<int>& array)
    {
        size = array.size();
        tree.reserve(2 * size);
        for (int i = 0; i < size; ++i)
        {
            tree[size + i] = array[i];
        }
        for (int i = size - 1; i > 0; --i)
        {
            tree[i] = std::min(tree[i << 1], tree[i << 1 | 1]);
        }
    }
    void set(int index, int value)
    {
        index += size;
        tree[index] = value;

        for (; index > 1; index >>= 1)
        {
            tree[index >> 1] = std::min(tree[index], tree[index ^ 1]);
        }
    }
    int rmq(int l, int r)
    {
        int min = INF;
        for (l += size, r += size; l < r; l >>= 1, r >>= 1) 
        { 
            if (l & 1)
            {
                min = std::min(min, tree[l++]);
            }  
        
            if (r & 1)
            {
                min = std::min(min, tree[--r]); 
            }  
        }
        return min;
    }
private:
    std::vector<int> tree;
    int size;
};


int main()
{
    freopen("rmq.in", "r", stdin);
    freopen("rmq.out", "w", stdout);

    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(0);
    // std::cout.tie(0);
 
    int n, m;
    std::cin >> n >> m;
    std::vector<query> queries(m);
    std::vector<int> arr(n, INF);
    segment_tree DO(arr);
    for (int i = 0; i < m; i++)
    {
        std::cin >> queries[i].i >> queries[i].j >> queries[i].q;
    }
    
    std::sort(queries.begin(), queries.end(), [](auto&& a, auto&& b)
    {
        return a.q < b.q;
    });

    for (int i = 0; i < m; ++i)
    {
        for (int j = queries[i].i; j <= queries[i].j; j++)
        {
            arr[j - 1] = queries[i].q;
        }
    }
    for (int i = 0; i < n; ++i)
    {
        if (arr[i] != INF)
        {
            DO.set(i, arr[i]);
        }
    }
    

    for (int i = 0; i < m; ++i)
    {
        if (queries[i].q != DO.rmq(queries[i].i - 1, queries[i].j))
        {
            std::cout << "inconsistent";
            return 0;
        }
    }
    std::cout << "consistent\n";
    for (int i = 1; i <= n; ++i)
    {
        std::cout << DO.rmq(i - 1, i) << ' ';
    }
    
    fclose(stdin);
    fclose(stdout);
    
    return 0;
}