#pragma once

#define pr(s) printf("%s\n", s.c_str())

#include <cmath>
#include <string>
#include <map>

inline int is_symb(char c);
std::string remove_spaces(const std::string& str);
std::pair<std::string, std::string> split(const std::string& str);
std::string insert_variables(const std::string& S, std::map<std::string, double>& dict);

bool starts_with(const std::string& str, const std::string& temp);