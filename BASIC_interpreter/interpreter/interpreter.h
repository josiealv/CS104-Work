
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include "arithmetic.h"
#include "command.h"
#include <iostream>

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();
    void run(std::ostream& out);

private:
    NumericExpression* parse_expression(std::string& express, int& pos, 
        NumericExpression* left);
    NumericExpression* parse_const(std::string& express, int& pos);
    NumericExpression* parse_variable (std::string& express, int& pos);
    void parse(std::istream& in);

    std::map <int, Command*> commands; //stores our commands with the line number being the key
    std::map <std::string, int> var;
    std::map <std::string, std::map<int, int> > array;

    //gets the full expression without whitesapce  following
    //the command/variable name
    void remove_ws(std::stringstream& stream, 
    std::string& exp);
};

#endif