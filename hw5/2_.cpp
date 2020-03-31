#include <string>
#include <iostream>
#include <vector>

class segment_tree
{
public:
    explicit segment_tree(const std::vector<int64_t>& array)
    {
        size = array.size();
        tree.reserve(2 * size);
        for (size_t i = 0; i < size; ++i)
        {
            tree[size + i] = array[i];
        }
        for (size_t i = size - 1; i > 0; --i)
        {
            tree[i] = tree[i << 1] + tree[i << 1 | 1];
        }
    }
    void set(size_t index, int64_t value)
    {
        index += size;
        tree[index] = value;

        for (; index > 1; index >>= 1)
        {
            tree[index >> 1] = tree[index] + tree[index ^ 1];
        }
    }
    int64_t rsq(size_t l, size_t r)
    {
        int64_t sum = 0;
        for (l += size, r += size; l < r; l >>= 1, r >>= 1) 
        { 
            if (l & 1)
            {
                sum += tree[l++];
            }  
        
            if (r & 1)
            {
                sum += tree[--r]; 
            }  
        }
        return sum;
    }
private:
    std::vector<int64_t> tree;
    size_t size;
};

int main()
{
    int n;
    std::cin >> n;
    std::vector<int64_t> values(n);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> values[i];
    }

    segment_tree DO(values);

    std::string mod;
    int l, r;
    while(std::cin >> mod >> l >> r)
    {
        if (mod == "sum")
        {
            std::cout<< DO.rsq(l - 1, r) << std::endl;
        }
        else
        {
            DO.set(l - 1, r);
        }
    }
    
    return 0;
}