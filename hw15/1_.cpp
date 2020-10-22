#include <vector>
#include <iostream>

template<typename T>
class persistent_stack
{
    struct node
    {
        T value;
        int previous;
    };
    std::vector<node> nodes;
public:
    persistent_stack(int n)
    {
        nodes.reserve(n);
        nodes.push_back({T(), -1});
    }
    persistent_stack()
    {
        nodes.push_back({T(), -1});
    }
    void push(int i, T x)
    {
        nodes.push_back({x, i});
    }
    T pop(int i)
    {
        nodes.push_back(nodes[nodes[i].previous]);

        return nodes[i].value;
    } 
};

int main()
{
    int n;
    std::cin >> n;
    persistent_stack<int> stack(n);

    for (int i = 0; i < n; ++i)
    {
        int t, m;
        std::cin >> t >> m;
        if (m == 0)
        {
            std::cout << stack.pop(t) << '\n';
        }
        else
        {
            stack.push(t, m);
        }
        
    }
    
    return 0;
}