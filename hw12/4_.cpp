#include <vector>
#include <iostream>
#include <string>

const int MAX_LENGTH = 100'000;
const int ALPHABET_SIZE = 26;

class Solution
{
public:
    Solution()
        :   links(MAX_LENGTH, std::vector<int>(ALPHABET_SIZE, 0))
        ,   counts(MAX_LENGTH, 0)
        ,   indexes(MAX_LENGTH, - 1)
    {
    }
    void add(const std::string& s)
    {
        for (int i = 0; i < s.size(); ++i)
        {
            char c = tolower(s[i]) - 'a';
            int ns = terminals++;
            counts[ns] = ++count;
            int p = last;
            while (p != -1 && links[p][c] == 0)
            {
                links[p][c] = ns;
                p = indexes[p];
            }

            if (p == -1)
            {
                indexes[ns] = 0;
            }
            else
            {
                int q = links[p][c];
                if (counts[q] == counts[p] + 1)
                {
                    indexes[ns] = q;
                }
                else
                {
                    int r = terminals++;
                    for (int j = 0; j < links.front().size(); ++j)
                    {
                        links[r][j] = links[q][j];
                    }
                    
                    indexes[r] = indexes[q];
                    indexes[q] = indexes[ns] = r;
                    counts[r] = counts[p] + 1;  
                    while (p != -1 && links[p][c] == q)
                    {
                        links[p][c] = r;
                        p = indexes[p];
                    }
                }
            }

            last = ns;
        }
    }
    bool find(const std::string& s)
    {
        int cs = 0;
        bool ok = true;
        for (int i = 0; i < s.size(); i++)
        {
            if (!(cs = links[cs][tolower(s[i]) - 'a']))
            {
                ok = false;
                break;
            }
        }

        return ok;
    }
private:
    std::vector<std::vector<int>> links;
    std::vector<int> counts;
    std::vector<int> indexes;

    int terminals = 1;
    int last = 0;
    int count = 0;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::string s;
    Solution sol;
    char c;
    while (std::cin >> c >> s)
    {
        if (c == 'A')
        {
            sol.add(s);
        }
        else
        {
            std::cout << (sol.find(s) ? "YES" : "NO") << '\n';
        }
    }

    return 0;
}