#pragma once

#include <cmath>
#include <string>
#include <deque>
#include <map>

class Token
{
public:
    Token(std::string data)
        :   value(data)
    {
    };
    bool is_terminal(std::string input)
    {
        return (input == get_value()) ? true : false;
    }
    std::string get_value()
    {
        return value;
    }
    double get_double()
    {
        std::size_t strSize;
        double num = stod(value, &strSize);
        return num;
    }

private:
    std::string value;
};


std::deque<Token> tokenize(std::string input);
inline int is_symb(char c);
std::string remove_spaces(const std::string& str);
std::pair<std::string, std::string> split(const std::string& str);
std::string insert_variables(const std::string& S, std::map<std::string, double>& dict);

bool starts_with(const std::string& str, const std::string& temp);