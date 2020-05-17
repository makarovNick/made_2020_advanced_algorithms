#include "../include/parser.h"

double ArithmeticParser::parse(std::string input)
{
    std::deque<Token> tokens = tokenize(input);

    double out = parse_expr(tokens);
    if (tokens.front().get_value() != "end")
    {
        throw std::runtime_error("Error: invalid input\n");
    }
    else if (std::isnan(out) || std::isinf(out))
    {
        throw std::runtime_error("Arithmetic error: divission by 0\n");
    }
    else
    {
        return out;
    }
}

double ArithmeticParser::parse_expr(std::deque<Token>& input)
{
    double out;
    out = parse_term(input);
    Token term1 = input.front();
    while (term1.is_terminal("+") || term1.is_terminal("-"))
    {
        input.pop_front();
        double term2 = parse_term(input);
        if (term1.is_terminal("+"))
        {
            out += term2;
        }
        else
        {
            out -= term2;
        }
        term1 = input.front();
    }
    return out;
}
double ArithmeticParser::parse_term(std::deque<Token>& input)
{
    double out;
    out = parse_factor(input);
    Token factor1 = input.front();
    while (factor1.is_terminal("*") || factor1.is_terminal("/"))
    {
        input.pop_front();
        double factor2 = parse_factor(input);
        if (factor1.is_terminal("*"))
        {
            out *= factor2;
        }
        else if (factor1.is_terminal("/"))
        {
            out /= factor2;
        }
        factor1 = input.front();
    }
    return out;
}
double ArithmeticParser::parse_factor(std::deque<Token>& input)
{
    double out;
    int sign = 1;
    Token temp = input.front();
    if (temp.is_terminal("-"))
    {
        sign = -1;
    }
    if (sign < 0)
    {
        input.pop_front();
    }
    out = parse_item(input);

    return out * sign;
}
double ArithmeticParser::parse_item(std::deque<Token>& input)
{
    double expr;
    Token item = input.front();
    input.pop_front();
    bool isNumber = true;
    for (char c : item.get_value())
    {
        if (!(isdigit(c) || c == '.' || c == ','))
        {
            isNumber = false;
        }
    }
    if (item.is_terminal("+") == true)
    {
        throw std::runtime_error("Arithmetic error: + before number\n");
    }
    if (isNumber)
    {
        return item.get_double();
    }
    if (item.is_terminal("(") != true)
    {
        throw std::runtime_error("Arithmetic error: bad number\n");
    }
    expr = parse_expr(input);
    if (input.front().is_terminal(")") != true)
    {
        throw std::runtime_error("Arithmetic error: empty input\n");
    }
    input.pop_front();

    return expr;
}

void parse_assigment(std::string& expr, std::map<std::string, double>& dict)
{
    if (expr.size() == 0)
    {
        return;
    }
    std::string name;
    int offset = 0;
    while (isspace(expr[offset]))
    {
        offset++;
        if (offset == expr.size() - 1)
        {
            return;
        }
    }
    while (expr[offset] != '=' && !isspace(expr[offset]))
    {
        name.push_back(expr[offset]);
        offset++;
    }
    while (isspace(expr[offset]))
    {
        offset++;
    }
    expr = insert_variables(std::string(expr.begin() + offset + 1, expr.end()), dict);
    ArithmeticParser parser;

    dict[name] = parser.parse(expr);
}


bool parse_if(std::string& start, std::istream& is, std::map<std::string, double>& dict)
{
    std::string condition;
    int offset = 0;
    while (isspace(start[offset]))
    {
        offset++;
    }
    offset += 2;
    while (isspace(start[offset]))
    {
        offset++;
    }
    for (int i = offset; i < start.length(); ++i)
    {
        if (!isspace(start[i]))
        {
            condition.push_back(start[i]);
        }
    }
    condition = insert_variables(condition, dict);
    ArithmeticParser parser;
    bool res = int(parser.parse(condition));
    if (!std::getline(is, condition) || remove_spaces(condition) != "{")
    {
        throw std::runtime_error("Condition parse error: expected bracket '{'");
    }
    bool is_true = false;
    bool is_if_prev = false;
    while (std::getline(is, condition))
    {
        if (starts_with(condition, "}"))
        {
            break;
        }
        if (res)
        {
            parse(condition, is, dict, is_true, is_if_prev);
        }
    }
}

void parse_else(std::istream& is, std::map<std::string, double>& dict, bool done)
{
    std::string condition;
    bool to_do = false;
    if (!std::getline(is, condition) || remove_spaces(condition) != "{")
    {
        throw std::runtime_error("Condition parse error: expected bracket '{'");
    }
    bool is_true = false;
    bool is_if_prev = false;
    while (std::getline(is, condition))
    {
        if (starts_with(condition, "}"))
        {
            break;
        }
        if (!done)
        {
            parse(condition, is, dict, is_true, is_if_prev);
        }
    }
}

void parse_while(std::string& start, std::istream& is, std::map<std::string, double>& dict)
{
    std::string condition;
    int offset = 0;
    while (isspace(start[offset]))
    {
        offset++;
    }
    offset += 5;
    while (isspace(start[offset]))
    {
        offset++;
    }
    for (int i = offset; i < start.length(); ++i)
    {
        if (!isspace(start[i]))
        {
            condition.push_back(start[i]);
        }
    }
    std::deque<std::string> assigments;    
    ArithmeticParser parser;
    std::string temp;
    if (!std::getline(is, temp) || remove_spaces(temp) != "{")
    {
        throw std::runtime_error("Condition parse error: expected bracket '{'");
    }
    int count = 1;
    while (std::getline(is, temp))
    {
        if (remove_spaces(temp) == "{")
        {
            count++;
        }
        if (remove_spaces(temp) == "}")
        {
            count--;
        }
        if(count == 0)
        {
            break;
        }
        if (count < 0)
        {
            throw std::runtime_error("Condition parse error: unexpected bracket '}'");
        }
        assigments.push_back(temp);
    }
    
    bool is_true = false;
    bool is_if_prev = false;
    while (parser.parse(insert_variables(condition, dict)))
    {
        std::stringstream ss;
        std::copy(assigments.begin(), assigments.end(), std::ostream_iterator<std::string>(ss, "\n"));
        while(std::getline(ss, temp))
        {
            parse(temp, ss, dict, is_true, is_if_prev);
        }
    }
}

int parse(std::string& start, std::istream& is, std::map<std::string, double>& dict, bool& is_true, bool& is_if_prev)
{
    if (starts_with(start, "if"))
    {
        is_true = parse_if(start, is, dict);
        is_if_prev = true;
    }
    else if (starts_with(start, "else"))
    {
        if (is_if_prev)
        {
            parse_else(is, dict, is_true);
        }
        else
        {
            throw std::runtime_error("'else' without 'if'\n");
        }
        is_true = false;
        is_if_prev = false;
    }
    else if (starts_with(start, "while"))
    {
        parse_while(start, is, dict);
    }
    else
    {
        parse_assigment(start, dict);
        is_if_prev = false;
    }
}