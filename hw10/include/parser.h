#include "../include/lexer.h"

#include <sstream>
#include <iterator>
#include <stdexcept>
#include <istream>

class ArithmeticParser
{
public:
    double parse(std::string input);

private:
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