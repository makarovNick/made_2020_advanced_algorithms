#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "../include/parser.h"
#include "../include/lexer.h"

SCENARIO("ARITHMETIC EXPRESSION")
{
    ArithmeticParser parser;
    WHEN("NUMBER")
    {
        THEN("OK")
        {
            REQUIRE(parser.parse("10") == 10);
        }
        THEN("OK")
        {
            REQUIRE(parser.parse("(10)") == 10);
        }
    }
    WHEN("SIMPLE EXPRESSION")
    {
        THEN("OK")
        {
            REQUIRE(parser.parse("10        +  13 -4") == 19);
        }
        THEN("OK")
        {
            REQUIRE(parser.parse("12 - 4 * 3") == 0);
        }
        THEN("OK")
        {
            REQUIRE(parser.parse("4 - 6/ 2*  3") == -5);
        }
    }
    WHEN("COMPLEX EXPRESSION")
    {
        THEN("OK")
        {
            REQUIRE(parser.parse("123+345-((((  11  +3)-13*2/2)-12*2-2)-2)*2") == 522.0);
        }
        THEN("OK")
        {
            REQUIRE(parser.parse("2*(123 * 2* 3/2*9-(123- (23-12)*13*(12-23*(23))/2))") == -67535.0);
        }
    }
    WHEN("WRONG EXPRESSION")
    {
        THEN("EXCEPT")
        {
            REQUIRE_THROWS(parser.parse("12-"));
        }
        THEN("EXCEPT")
        {
            REQUIRE_THROWS(parser.parse("((12 - 2) -123))"));
        }
    }
    WHEN("ZERO DIVISION")
    {
        THEN("EXCEPT")
        {
            REQUIRE_THROWS(parser.parse("1/0"));
        }
    }
}

SCENARIO("LEXER")
{
    WHEN("SPACES")
    {
        THEN("OK")
        {
            REQUIRE(remove_spaces("a = 1 + 2 / 2 - 4") == "a=1+2/2-4");
        }
    }
    WHEN("SYMBOL")
    {
        THEN("OK")
        {
            REQUIRE(is_symb('1'));
            REQUIRE(is_symb('+'));
        }
        THEN("OK")
        {
            REQUIRE(!is_symb('a'));
        }
    }
    WHEN("SPLIT")
    {
        THEN("OK")
        {
            REQUIRE(split("a=10").first == "a");
            REQUIRE(split("a=10").second == "10");
        }
    }
    WHEN("INSERT VARIABLES")
    {
        THEN("OK")
        {
            std::map<std::string, double> dict;
            dict["a"] = 100;
            dict["b"] = 200;
            REQUIRE(insert_variables("123+a", dict) == "123+1000/10");
            REQUIRE(insert_variables("123+a+b", dict) == "123+1000/10+2000/10");
        }
    }
}

SCENARIO("PARSER")
{
    WHEN("ASSIGMENT")
    {
        std::map<std::string, double> dict;
        dict["abc"] = 1023.0;
        dict["kek"] = 124.0;
        THEN("OK")
        {
            std::string str = "a = abc + kek * 10 - (202)";

            parse_assigment(str, dict);
            REQUIRE(dict["a"] == 2061);
        }
        THEN("OK")
        {
            std::string str = "b = abc";

            parse_assigment(str, dict);
            REQUIRE(dict["b"] == 1023);
        }
    }
    WHEN("IF")
    {
        std::map<std::string, double> dict;
        THEN("OK")
        {
            std::stringstream ss;
            ss << "{\n" << "b = 100 / 2\n" << "}\n";

            std::string str = "if (100)";

            parse_if(str, ss, dict);
            REQUIRE(dict["b"] == 50);
        }
        THEN("OK")
        {
            std::stringstream ss;
            ss << "{\n" << "c = 33 / 11\n" << "}\n";
            dict["abc"] = 1022;
            std::string str = "if( abc )";

            parse_if(str, ss, dict);
            REQUIRE(dict["c"] == 3);

            std::cout << dict["c"];
        } 
    }
    WHEN("ELSE")
    {
        std::map<std::string, double> dict;
        std::stringstream ss;
        THEN("OK")
        {
            ss << "{\n" << "c = 13\n" << "}\n";
            parse_else(ss, dict, false);
            REQUIRE(dict["c"] == 13);
        }
    }
    WHEN("WHILE")
    {
        std::map<std::string, double> dict;
        dict["a"] = 100;
        std::stringstream ss;
        THEN("OK")
        {
            ss << "{\n" << "a = a - 1\n" << "}\n";
            std::string str = "while(a)\n";
            parse_while(str, ss, dict);
            REQUIRE(dict["a"] == 0);
        }
    }
}