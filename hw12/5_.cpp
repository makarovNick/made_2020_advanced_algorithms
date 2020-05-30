#include <bits/stdc++.h>

const int ALPHABET_SIZE = 27;

class suffix_tree
{
    struct suffix_node
    {
        
        suffix_node* children[ALPHABET_SIZE];
        suffix_node* suff_link;
        int start;
        int *end;
    };
public:
    suffix_tree(std::string&);
    std::vector<int> suffix_array()
    {
        DFS(root, 0);

        return sa;
    }
private:
    suffix_node* new_node(int, int*);
    int edge_length(suffix_node*);
    void DFS(suffix_node*, int);

    std::vector<int> sa;

    suffix_node *root = nullptr;
    suffix_node *prev_node = nullptr;
    suffix_node *current_node = nullptr;
    int current_edge = -1;
    int current_length = 0;
    int suf_count = 0;

    int size;
};

suffix_tree::suffix_tree(std::string& s)
{
    s += '`';
    size = s.size();

    root = new_node(-1, new int(-1));
    current_node = root;

    for (int i = 0; i < size; i++)
    {
        suf_count++;
        prev_node = nullptr;
        while (suf_count > 0)
        {
            if (current_length == 0)
            {
                current_edge = i;
            }

            if (current_node->children[s[current_edge] - '`'] == nullptr)
            {
                current_node->children[s[current_edge] - '`'] = new_node(i, &i);
                if (prev_node != nullptr)
                {
                    prev_node->suff_link = current_node;
                    prev_node = nullptr;
                }
            }
            else
            {
                suffix_node* next = current_node->children[s[current_edge] - '`'];
                if (current_length >= edge_length(next))
                {
                    current_edge += edge_length(next);
                    current_length -= edge_length(next);
                    current_node = next;
                    continue;
                }
                if (s[next->start + current_length] == s[i])
                {
                    if (prev_node != nullptr && current_node != root)
                    {
                        prev_node->suff_link = current_node;
                        prev_node = nullptr;
                    }

                    current_length++;
                    break;
                }

                suffix_node* split = new_node(next->start, new int(next->start + current_length - 1));
                current_node->children[s[current_edge] - '`'] = split;

                split->children[s[i] - '`'] = new_node(i, &i);
                next->start += current_length;
                split->children[s[next->start] - '`'] = next;

                if (prev_node != nullptr)
                {
                    prev_node->suff_link = split;
                }
                prev_node = split;
            }
            suf_count--;
            if (current_node == root && current_length > 0)
            {
                current_length--;
                current_edge = i - suf_count + 1;
            }
            else if (current_node != root)
            {
                current_node = current_node->suff_link;
            }
        }
    }
    s.pop_back();
}

suffix_tree::suffix_node* suffix_tree::new_node(int start, int* end)
{
    suffix_node* temp = new suffix_node;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        temp->children[i] = nullptr;

    temp->suff_link = root;
    temp->start = start;
    temp->end = end;

    return temp;
}

void suffix_tree::DFS(suffix_node* root, int height)
{
    if (root == nullptr)
        return;

    if (*(root->end) == (size - 1))
    {
        // print(*(root->end) - height + 1, *(root->end));
    }
    bool leaf = true;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != nullptr)
        {
            leaf = false;
            DFS(root->children[i], height + edge_length(root->children[i]));
        }
    }
    if (leaf)
    {
        sa.push_back(static_cast<int>(size) - height + 1);
    }
}

int suffix_tree::edge_length(suffix_node* n)
{
    return *(n->end) - (n->start) + 1;
}

std::vector<int> lcp_kasai(const std::string& txt, const std::vector<int>& suffixArr)
{
    int n = suffixArr.size();

    std::vector<int> lcp(n);
    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i)
    {
        pos[suffixArr[i]] = i;
    }

    int current_length = 0;
    for (int i = 0; i < n; ++i)
    {
        if (pos[i] == n - 1)
        {
            current_length = 0;
        }
        else
        {
            int j = suffixArr[pos[i] + 1];

            while (i + current_length < n && j + current_length < n && txt[i + current_length] == txt[j + current_length])
            {
                current_length++;
            }

            lcp[pos[i]] = current_length;
            if (current_length > 0)
            {
                current_length--;
            }
        }
    }

    return lcp;
}

int main()
{
    std::string s;
    std::cin >> s;
    suffix_tree st(s);
    auto sa = st.suffix_array();
    for (int i = 1; i < sa.size(); ++i)
    {
        std::cout << sa[i] + 1 << ' ';
    }
    auto lcp  = lcp_kasai(s, sa);
    std::cout << '\n';
    for (int i = 1; i < lcp.size() - 1; ++i)
    {
        std::cout << lcp[i] << ' ';
    }
    
    return 0;
}