#include "../include/parser.h"
#include "../include/cxxopts.hpp"

#include <unordered_map>
#include <vector>

int main(int argc, char** argv)
{
    cxxopts::Options options("test", "A brief description");

    options.add_options()
        ("f,file", "Read from file", cxxopts::value<std::string>())
        ("o,output", "Write from file", cxxopts::value<std::string>())
    ;

    auto result = options.parse(argc, argv);

    if (result.count("file"))
    {
        auto path = result["file"].as<std::string>();
        freopen(path.c_str(), "r", stdin);
    }
    if (result.count("output"))
    {
        auto path = result["output"].as<std::string>();
        freopen(path.c_str(), "w+", stdout);
    }

    std::map<std::string, double> variables;
    std::string expres;
    bool is_if_prev = false;
    bool is_true = false;
    while(std::getline(std::cin, expres))
    {
        parse(expres, std::cin, variables, is_true, is_if_prev);
    }

    for (auto &i : variables)
    {
        std::cout << i.first << " = " << i.second << '\n';
    }

    return 0;
}