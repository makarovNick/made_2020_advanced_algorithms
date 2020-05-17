#include "../include/lexer.h"

inline int is_symb(char c)
{
    return c == '(' || c == ')' || c == '+' || c == '-' || c == '/' || c == '*' || isdigit(c) || isspace(c) || c == '.' || c == ',';
}

std::pair<std::string, std::string> split(const std::string& str)
{
    int pos;
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == '(' || str[i] == '=')
        {
            pos = i;
            break;
        }
    }

    return std::make_pair(std::string(str.begin(), str.begin() + pos) , std::string(str.begin() + pos + 1, str.end()));
}

std::deque<Token> tokenize(std::string input)
{
    std::deque<Token> tokens;
    for (int i = 0; i < input.size(); ++i)
    {
        if (!(isdigit(input[i]) || input[i] == '.' || input[i] == ','))
        {
            if (isspace(input[i]) && input[i] != '\n')
            {
                continue;
            }
            std::string temp;
            temp.push_back(input[i]);
            tokens.push_back(Token(temp));
        }
        else
        {
            std::deque<char> buffer;
            while (isdigit(input[i]) || input[i] == '.' || input[i] == ',')
            {
                buffer.push_back(input[i]);
                ++i;
            }
            i--;
            std::string number;
            for (int j = 0; j < buffer.size(); ++j)
            {
                number.push_back(buffer[j]);
            }
            tokens.push_back(Token(number));
        }
    }
    tokens.push_back(Token("end"));
    return tokens;
}

std::string remove_spaces(const std::string& str)
{
    std::string res;
    for(auto& c: str)
    {
        if(!isspace(c))
        {
            res.push_back(c);
        }
    }
    return res;
}

std::string insert_variables(const std::string& S, std::map<std::string, double>& dict)
{
    std::string res;
    std::string temp;
    for (int i = 0; i < S.size(); ++i)
    {
        if (!is_symb(S[i]))
        {
            temp.push_back(S[i]);
        }
        else
        {
            if (temp.size() != 0)
            {
                if (dict.find(temp) == dict.end())
                {
                    throw std::runtime_error("Error : uninitialized variable\n");
                }
                res += std::to_string(int(dict[temp] * 10));
                res += "/10";   // crutch
                temp.clear();
            }
            res.push_back(S[i]);
        }
    }
    if (temp.size() != 0)
    {
        res += std::to_string(int(dict[temp] * 10));
        res += "/10";   // crutch
    }

    return res;
}
bool starts_with(const std::string& str, const std::string& temp)
{
    int offset = 0;
    while(isspace(str[offset]))
    {
        offset++;
    }
    if (str.size() - offset >= temp.size())
    {
        for (int i = 0; i < temp.size(); ++i)
        {
            if (str[i + offset] != temp[i])
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    
    return true;
}