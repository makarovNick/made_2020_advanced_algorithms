#include <iostream>
#include <vector>
 
template<typename T>
class persistent_array
{
    struct node
    {
        node* left;
        node* right;
        T val;
    };

public:
    persistent_array(const std::vector<T>& other)
    {
        size = other.size();
        versions.push_back(new node());
	    build(versions.front(), other, 1, size);
    }
    void create(int i, int j, int x)
    {
        versions.push_back(new node());
        add(versions.back(), versions[i - 1], 1, size, j, x);
    }
    T get(int i, int j)
    {
        node* node = versions[i - 1];
        int l = 1;
        int r = size;
        while (l != r)
        {
            int m = (l + r) / 2;
            if (j <= m)
            {
                r = m;
                node = node->left;
            }
            else
            {
                l = m + 1;
                node = node->right;
            }
        }

        return node->val;
    }
private:
    void build(node* current, const std::vector<T>& arr, int l, int r)
    {
        if (l == r)
        {
            current->val = arr[l-1];
        }
        else
        {
            int m = (l + r) / 2;
            current->left = new node();
            build(current->left, arr, l, m);
            current->right = new  node();
            build(current->right, arr, m + 1, r);
        }
    }
    
    void add(node* to, node* from, int l, int r, int j, int x)
    {
        if (l == r)
        {
            to->val = x;
        }
        else
        {
            int m = (l + r) / 2;
            if (j <= m)
            {
                to->right = from->right;
                node* left = new node();
                add(left, from->left, l, m, j, x);
                to->left = left;
            }
            else
            {
                to->left = from->left;
                node* right = new node();
                add(right, from->right, m + 1, r, j, x);
                to->right = right;
            }
        }
    }
private:
    int size;
	std::vector<node*> versions;
};

int main() 
{
	std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

	int n;
	std::cin >> n;
	std::vector<int> arr(n);
	for (int i = 0; i < n; ++i)
    {
		std::cin >> arr[i];
    }
    persistent_array<int> stack(arr);

	std::cin >> n;
	for (int z = 0; z < n; ++z)
	{
        int i, j;
		std::string request;
		std::cin >> request;
		if (request[0] == 'c')
		{
            int x;
			std::cin >> i >> j >> x;
            stack.create(i, j, x);
		}
		else
		{
			std::cin >> i >> j;
			std::cout << stack.get(i, j) << '\n';
		}
	}

	return 0;
}