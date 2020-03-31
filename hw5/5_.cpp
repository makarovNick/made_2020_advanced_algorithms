#include <vector>
#include <iostream>
 
struct matrix
{
    int a, b, c, d;
};
 
// void log(const matrix& res)
// {
//     std::cout << res.a << ' ' << res.b << std::endl;
//     std::cout << res.c << ' ' << res.d << std::endl << std::endl;
// }
 
matrix mul_mod(const matrix& first, const matrix& other, int m )
{
    return {((first.a * other.a)% m + (first.b * other.c)% m) % m, ((first.a * other.b)% m + (first.b * other.d)% m) % m,
            ((first.c * other.a)% m + (first.d * other.c)% m) % m, ((first.c * other.b)% m + (first.d * other.d)% m) % m};
}
class segment_tree
{
public:
    explicit segment_tree(const std::vector<matrix>& array, int _m)
    {
        m = _m;
        size = array.size();
        tree.reserve(2 * size);
        for (size_t i = 0; i < size; ++i)
        {
            tree[size + i] = array[i];
        }
        for (size_t i = size - 1; i > 0; --i)
        {
            tree[i] = mul_mod(tree[i << 1], tree[i << 1 | 1], m);
        }
    }
    matrix rsq(size_t l, size_t r)
    {
        matrix l_elem = {1, 0, 0, 1};
        matrix r_elem = {1, 0, 0, 1};
        for (l += size, r += size; l < r; l >>= 1, r >>= 1) 
        { 
            if (l & 1)
            {
                l_elem = mul_mod(l_elem, tree[l++], m);
            }
        
            if (r & 1)
            {
                r_elem = mul_mod(tree[--r], r_elem, m); 
            }  
        }
        return mul_mod(l_elem, r_elem, m);
    }
private:
    std::vector<matrix> tree;
    int m;
    size_t size;
};
 
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    freopen("crypto.in", "r", stdin);
    freopen("crypto.out", "w", stdout);
    int r, n, m;
    std::cin >> r >> n >> m;
    std::vector<matrix> arr(n);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> arr[i].a >> arr[i].b >> arr[i].c >> arr[i].d;
    }
    segment_tree DO(arr, r);
    for (int i = 0; i < m; ++i)
    {
        int l, r;
        std::cin >> l >> r;
        matrix res = DO.rsq(l - 1, r);
        std::cout << res.a << ' ' << res.b << '\n';
        std::cout << res.c << ' ' << res.d << "\n\n";
    }
    
    return 0;
}