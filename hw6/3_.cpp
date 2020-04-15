#include <iostream>
#include <vector>
#include <algorithm>

const int MAX_N = 2000000;

int left_bound = 200000;
int right_bound = -200000;
int current = 0;

struct Vertical
{
    int x, y_upper, y_lower;
    bool is_left;
};

class SegmentTree
{
    struct node
    {
        int value;
        int pending_value;
    };

public:
    SegmentTree(const std::vector<Vertical>& other)
        : field(other)
        , nodes(3000000)
    {
        // field.resize(field.size() * 4);
        // nodes.resize(field.size());
    }

    void check(int& a, int& b, int& c)
    {
        if (nodes[0].value > a)
        {
            a = nodes[0].value;
            b = field[current].x;
            c = __rmq(0, 0, right_bound);
        }
    }

    void update(int x, int l, int r)
    {
        if (nodes[x].pending_value != 0)
        {
            __push(x, l, r);
        }
        if ((field[current].y_lower - left_bound <= r) && (field[current].y_upper - left_bound >= l))
        {
            if (nodes[(x << 1) + 1].pending_value != 0)
            {
                __push((x << 1) + 1, l, (l + r) >> 1);
            }
            if (nodes[(x << 1) + 2].pending_value != 0)
            {
                __push((x << 1) + 2, ((l + r) >> 1) + 1, r);
            }
            if ((l >= field[current].y_lower - left_bound) && ((r <= field[current].y_upper - left_bound)))
            {
                nodes[x].value += (field[current].is_left ? 1 : -1);
                if (l != r)
                {

                    nodes[(x << 1) + 1].pending_value += (field[current].is_left ? 1 : -1);
                    nodes[(x << 1) + 2].pending_value += (field[current].is_left ? 1 : -1);
                }
                nodes[x].pending_value = 0;
                while (x != 0)
                {
                    x = (x - 1) >> 1;
                    nodes[x].value = std::max(nodes[(x << 1) + 1].value, nodes[(x << 1) + 2].value);
                }
            }
            else
            {
                update((x << 1) + 1, l, (l + r) >> 1);
                update((x << 1) + 2, ((l + r) >> 1) + 1, r);   
            }
        }
    }


private:
    void __push(int x, int l, int r)
    {
        nodes[x].value += nodes[x].pending_value;
        if (l != r)
        {
            nodes[(x << 1) + 1].pending_value += nodes[x].pending_value;
            nodes[(x << 1) + 2].pending_value += nodes[x].pending_value;
        }
        nodes[x].pending_value = 0;
    }

    int __rmq(int x, int l, int r)
    {
        if (nodes[x].pending_value != 0)
        {
            __push(x, l, r);
        }
        if (l == r)
        {
            return l;
        }
        if (nodes[(x << 1) + 1].pending_value != 0)
        {
            __push((x << 1) + 1, l, (l + r) >> 1);
        }
        if (nodes[(x << 1) + 2].pending_value != 0)
        {
            __push((x << 1) + 2, ((l + r) >> 1) + 1, r);
        }
        if (nodes[(x << 1) + 1].value == nodes[x].value)
        {
            return __rmq((x << 1) + 1, l, (l + r) >> 1);
        }
        return __rmq((x << 1) + 2, ((l + r) >> 1) + 1, r);
    }

    std::vector<Vertical> field;
    std::vector<node> nodes;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<Vertical> field(MAX_N);

    int N;
    std::cin >> N;
    N = (N << 1) - 1;
    for (int i = 0; i < N; i += 2)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        left_bound = std::min(left_bound, y1);
        right_bound = std::max(right_bound, y2);

        field[i] = { x1, y2, y1, true };
        field[i + 1] = { x2, y2, y1, false };
    }
    right_bound -= left_bound;
    int crosses = 0;
    int x, y;
    std::sort(field.begin(), field.begin() + N + 1, [](auto& a, auto& b)
    {
        return ((a.x < b.x) || (a.x == b.x && a.is_left && !b.is_left));
    });
    
    SegmentTree tree(field);

    for (; current < N; ++current)
    {
        tree.update(0, 0, right_bound);
        tree.check(crosses, x, y);
    }

    std::cout << crosses << '\n' << x << ' ' << y + left_bound;

    return 0;
}