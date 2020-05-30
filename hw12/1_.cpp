#include <iostream>
#include <string>
#include <vector>
#include <stack>

static const int ALPHABET_SIZE = 26;

static inline int letter_num(char c)
{
    return c - 'a';
}

class suffix_tree
{
    struct suffix_node;

    struct suffix_edge
    {
        suffix_node* from;
        suffix_node* to;
        int left;
        int right;
        ~suffix_edge()
        {
            // if (from)
            // {
            //     delete from;
            //     from = nullptr;
            // }
            // if (to)
            // {
            //     delete to;
            //     to = nullptr;
            // }
        }
        suffix_edge(suffix_node* start, suffix_node* end)
            : from(start)
            , to(end)
            , left(0)
            , right(0)
        {
        }

        suffix_edge(suffix_node* start, suffix_node* end, int l, int r)
            : from(start)
            , to(end)
            , left(l)
            , right(r)
        {
        }
    };

private:
    struct suffix_node
    {
        int id;
        suffix_node* link;
        suffix_edge* kids[ALPHABET_SIZE];

        ~suffix_node()
        {
            for (int i = 0; i < ALPHABET_SIZE; ++i)
            {
                if (kids[i])
                {
                    delete kids[i];
                    kids[i] = nullptr;
                }
            }
            if (link)
            {
                delete link;
                link = nullptr;
            }
        }

        explicit suffix_node(int id)
            : id(id)
            , link(nullptr)
        {
            std::fill(kids, kids + ALPHABET_SIZE, nullptr);
        }
    };

    int node_count = 0;
    int depth = 0;
    suffix_node* root;

    void DFS(suffix_node* node, std::ostream& out)
    {
        if (node)
        {
            for (suffix_edge* edge : node->kids)
            {
                if (edge)
                {
                    out << node->id         << ' ' 
                        << edge->to->id     << ' ' 
                        << edge->left + 1   << ' '
                        << edge->right      << '\n';
                    DFS(edge->to, out);
                }
            }
        }
    }

public:

    ~suffix_tree()
    {
        delete root;
    }

    suffix_tree(const std::string& s)
        :   root(new suffix_node(++node_count))
    {
        suffix_node* current = root;
        for (int i = 0; i < s.size(); i++)
        {
            suffix_node* previous = nullptr;
            depth++;
            while (depth > 0)
            {
                suffix_edge* edge;
                while (true)
                {
                    edge = current->kids[letter_num(s[i - depth + 1])];
                    if (!edge)
                    {
                        break;
                    }
                    int len = edge->right - edge->left;
                    if (depth <= len)
                    {
                        break;
                    }
                    depth -= len;
                    current = edge->to;
                }

                edge = current->kids[letter_num(s[i - depth + 1])];
                if (edge)
                {
                    int next_char = edge->left + depth - 1;
                    if (s[next_char] == s[i])
                    {
                        if (previous)
                        {
                            previous->link = current;
                        }

                        break;
                    }
                    else
                    {
                        auto* new_node = new suffix_node(++node_count);
                        auto* new_node2 = new suffix_node(++node_count);

                        new_node->kids[letter_num(s[next_char])] = new suffix_edge(new_node, edge->to, next_char, edge->right);
                        new_node->kids[letter_num(s[i])] = new suffix_edge(new_node, new_node2, i, s.size());

                        edge->right = next_char;
                        edge->to = new_node;
                        
                        if (previous)
                        {
                            previous->link = new_node;
                        }
                        previous = new_node;
                    }
                }
                else
                {
                    auto* new_node = new suffix_node(++node_count);

                    current->kids[letter_num(s[i])] = new suffix_edge(current, new_node, i, s.size());

                    if (previous)
                    {
                        previous->link = current;
                    }

                    previous = nullptr;
                }
                if (current == root)
                {
                    depth--;
                }
                else
                {
                    current = current->link;
                }
            }
        }
    }

    void DFS(std::ostream& out)
    {
        out << node_count << ' ' << node_count - 1 << '\n';

        DFS(root, out);
    }
};


int main()
{
    std::string s;
    std::cin >> s;

    suffix_tree st(s);

    st.DFS(std::cout);

    return 0;
}