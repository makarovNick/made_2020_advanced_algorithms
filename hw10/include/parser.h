#include "../include/lexer.h"

#include <sstream>
#include <iterator>
#include <stdexcept>
#include <istream>
#include <deque>

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

class ArithmeticParser
{
public:
    double parse(std::string input);

private:
    std::deque<Token> tokenize(std::string input);

    double parse_expr(std::deque<Token>& input);
    double parse_term(std::deque<Token>& input);
    double parse_factor(std::deque<Token>& input);
    double parse_item(std::deque<Token>& input);
};

bool parse_if(std::string& start, std::istream& is, std::map<std::string, double>& dict);
void parse_assigment(std::string& name, std::map<std::string, double>& dict);
void parse_else(std::istream& is, std::map<std::string, double>& dict, bool done);
void parse_while(std::string& start, std::istream& is, std::map<std::string, double>& dict);

int parse(std::string& start, std::istream& is, std::map<std::string, double>& dict, bool& is_true, bool& is_if_prev);