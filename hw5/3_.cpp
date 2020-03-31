#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
const int64_t INF = 1e18;
struct segment_tree
{
private:
    enum query
    {
        NO,
        SET,
        ADD
    };
    struct Node
    {
        int64_t min = 0;
        int64_t pending_value = 0;
        query pending_op = NO;
    };
	std::vector<Node>t;

    int size;
public:
    segment_tree(const std::vector<int64_t>& arr)
    {
        size = 1;
        while (size < arr.size())
        {
            size <<= 1;
        }
        t = std::vector<Node>(2 * size - 1);
        __build(arr, 0);
    }
    int64_t rmq(int l, int r)
    {
        return __rmq(l - 1, r - 1, 0, size - 1, 2 * size - 2);
    }
    void add(int l, int r, int64_t value)
    {
        __add(l - 1, r - 1, value, 0, size - 1, 2 * size - 2);
    }
    void set(int l, int r, int64_t value)
    {
        __set(l - 1, r - 1, value, 0, size - 1, 2 * size - 2);
    }
private:
    void __build(const std::vector<int64_t>& arr, int ind)
    {
        if (ind >= size - 1)
        {
            if ((ind + 1) % size > arr.size() - 1)
            {
                t[ind].min = INF;
            }
            else
            {
                t[ind].min = arr[(ind + 1) % size];                
            }
        }
        else
        {
            __build(arr, 2 * ind + 1);
            __build(arr, 2 * ind + 2);
            t[ind].min = std::min(t[2 * ind + 1].min, t[2 * ind + 2].min);
        }
    }
    inline void __propogate(int ind)
    {
        if (2 * ind + 2 < t.size())
        {
            switch (t[ind].pending_op)
            {
                case SET:
                    t[2 * ind + 1].min = t[ind].min;
                    t[2 * ind + 2].min = t[ind].min;
                    t[2 * ind + 1].pending_op = SET;
                    t[2 * ind + 2].pending_op = SET;
                    break;
                case ADD:
                    t[2 * ind + 1].min += t[ind].pending_value;
                    t[2 * ind + 2].min += t[ind].pending_value;
                    if (t[2 * ind + 1].pending_op != SET)
                    {
                        t[2 * ind + 1].pending_value += t[ind].pending_value;
                        t[2 * ind + 1].pending_op = ADD;
                    }
                    if (t[2 * ind + 2].pending_op != SET)
                    {
                        t[2 * ind + 2].pending_value += t[ind].pending_value;
                        t[2 * ind + 2].pending_op = ADD;
                    }
                    break;
            }
        }
        t[ind].pending_op = NO;
        t[ind].pending_value = 0;
    }
    int64_t __rmq(int l, int r, int ind, int seg_start, int seg_end)
    {
        __propogate(ind);

        int mid = ((t.size() + 1) / 2);
        if (l <= (seg_start + 1) % mid && r >= (seg_end + 1) % mid)
        {
            return t[ind].min;
        }
        if ((seg_start + 1) % mid > r || (seg_end + 1) % mid< l)
        {
            return INF;
        }
        mid = (seg_end - seg_start + 1) / 2;
        return std::min(__rmq(l, r, 2 * ind + 1, seg_start, seg_end - mid),__rmq(l, r, 2 * ind + 2, seg_start + mid, seg_end));
    }
    void __set(int l, int r, int64_t val, int ind, int seg_start, int seg_end)
    {
        __propogate(ind);
        if (l <= (seg_start + 1) % ((t.size() + 1) / 2) && r >= (seg_end + 1) % ((t.size() + 1) / 2))
        {
            t[ind].min = val;
            t[ind].pending_op = SET;
            return;
        }
        if ((seg_start + 1) % ((t.size() + 1) / 2) > r || (seg_end + 1) % ((t.size() + 1) / 2) < l)
        {
            return;
        }
        __set(l, r, val, 2 * ind + 1, seg_start, seg_end - (seg_end - seg_start + 1) / 2);
        __set(l, r, val, 2 * ind + 2, seg_start + (seg_end - seg_start + 1) / 2, seg_end);
        t[ind].min = std::min(t[2 * ind + 1].min, t[2 * ind + 2].min);
    }
    void __add(int l, int r, int64_t val, int ind, int seg_start, int seg_end)
    {
        __propogate(ind);
        if (l <= (seg_start + 1) % ((t.size() + 1) / 2) && r >= (seg_end + 1) % ((t.size() + 1) / 2))
        {
            t[ind].min += val;
            t[ind].pending_op = ADD;
            t[ind].pending_value = val;
            return;
        }
        if ((seg_start + 1) % ((t.size() + 1) / 2) > r || (seg_end + 1) % ((t.size() + 1) / 2) < l)
        {
            return;
        }
        __add(l, r, val, 2 * ind + 1, seg_start, seg_end - (seg_end - seg_start + 1) / 2);
        __add(l, r, val, 2 * ind + 2, seg_start + (seg_end - seg_start + 1) / 2, seg_end);
        t[ind].min = std::min(t[2 * ind + 1].min, t[2 * ind + 2].min);
    }
};

int main()
{
    int64_t n;
    std::cin >> n;
    std::vector<int64_t> a(n);
    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
    segment_tree DO(a);
    std::string mod;
    int i, j, x;
    while (std::cin >> mod)
    {
        std::cin >> i >> j;
        if (mod == "min")
        {
            std::cout << DO.rmq(i, j) << std::endl;
        }
        if (mod == "set")
        {
            std::cin >> x;
            DO.set(i, j, x);
        }
        if (mod == "add")
        {
            std::cin >> x;
            DO.add(i, j, x);
        }
    }
    return 0;
}