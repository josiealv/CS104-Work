#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "booleanExpression.h"
#include "arithmetic.h"
#include <string>
#include <map>


class Command {
    public:
        Command();
        virtual ~Command();
        virtual std::string format() const = 0;
        virtual std::string getType() const=0; //returns a string of a command based on subclass
   protected:
     int line_number;
};

class Print: public Command {
    public:
        Print(int line_num, NumericExpression* expression);
        ~Print();
        virtual std::string format() const; 
        int getExpression(std::map <std::string, int>& var,
    std::map<std::string, std::map <int, int> >& array);
        virtual std::string getType() const;

     private:
        NumericExpression* expression;
        std::string changing_exp;         
};

class Let: public Command {
    public:
        Let (int line_num, NumericExpression* name, 
        NumericExpression* value);
        ~Let();
         virtual std::string format() const;
         void setValue(std::map <std::string, int>& var, 
         std::map <std::string, std::map <int, int> >& array);
         virtual std::string getType() const;
    private:
        std::string new_format_for_var;
        NumericExpression* value;
        NumericExpression* name;
};

class GoTo: public Command
{
    public:
        GoTo(int line_num, int& jline);
        ~GoTo();
        virtual std::string format() const;
        virtual int getJline() const; //returns the line the program wants to jump to
        virtual std::string getType() const;
    private:
         int jump2;
};

class IfThen: public Command {
    public: 
        IfThen(int line_num, booleanExpression* bool_exp,
            std::string& jline);
        ~IfThen();
        virtual std::string format() const;
        virtual int getJline() const; //returns the line the program wants to jump to if getBooleanValue = true
        bool getBooleanValue(std::map <std::string, int>& var,
    std::map<std::string, std::map <int, int> >& array) const;
        virtual std::string getType() const;
    private:
        booleanExpression* boolean; //boolean pointer that points to the boolean class 
        std::string pretty_bool_exp;
        int jump;    
    
};

class GoSub: public Command {
    public:
        GoSub(int line_num, int& jline);
        ~GoSub();
        virtual std::string format() const;
        virtual int getJline() const;
        virtual std::string getType() const;

    private:
        int jump_to;    
};

class Return: public Command {
    public:
        Return(int line_num);
        ~Return();
        virtual std::string format() const;
        virtual std::string getType() const;

    private:
};

class End: public Command {
    public:
        End(int line_num);
        ~End();
        virtual std::string format() const;
        virtual std::string getType() const;
};

#endif