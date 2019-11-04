#include "command.h"
#include "arithmetic.h"
#include "booleanExpression.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Command::Command (){}
Command::~Command(){}

Print::Print(int line_num, NumericExpression* expression): expression(expression)
{
    line_number=line_num;
    
    changing_exp=expression->format();

}
Print::~Print(){
    delete this->expression;
}
string Print::getType()const
{
    return ("PRINT");
}
string Print::format( )const
{
   string new_exp=changing_exp;

   stringstream str_line; //converts line number back to string
   str_line<<line_number;

   return (str_line.str() +" PRINT "+ new_exp);
}
int Print::getExpression(std::map <std::string, int>& var,
    std::map<std::string, std::map <int, int> >& array)
{
    int expression_val = expression->getValue(var, array);
    return expression_val;
}

Let::Let(int line_num, NumericExpression* name,
        NumericExpression* value): name(name), value(value)
{
    line_number=line_num;
    new_format_for_var = name->format()+ " " + value->format();
}
void Let::setValue(map <string, int>& var, map <string, map<int, int> >& array)
{
    normalVar* v;
    v = dynamic_cast<normalVar* > (name);
    if (v) //if the expression for name is of normalVar type, call the setValue() function for it
    {
        v->setValue(value->getValue(var, array), var);
    }
    arrayVar* a;
    a = dynamic_cast<arrayVar* > (name);
    if(a) //if the expression for name is of arrayVar type, call the setValue() function for it
    {
        a->setValue(value->getValue(var, array), var, array);
    }
}
string Let::getType()const
{
    return ("LET");
}
string Let::format() const
{
    stringstream str_line; //convert line number to string
    str_line<<line_number;

    string pretty = new_format_for_var;

    return (str_line.str() + " LET "
    + pretty);
}
Let::~Let(){
    delete this->name;
    delete this->value;
}
IfThen::IfThen (int line_num, booleanExpression* bool_exp,
            std::string& jline): boolean(bool_exp)
{
    line_number = line_num;
    stringstream ss(jline); //stores jline into our jump variable
    ss>>jump;

    pretty_bool_exp = boolean->format();
}
string IfThen::getType()const
{
    return ("IF");
}
string IfThen::format()const
{
    stringstream str_jline; //convert jline back into a string
    str_jline<<jump;

    stringstream str_line; //same thing for line number
    str_line<<line_number;

  //adds in the line numbers to the boolean expression
  return (str_line.str()+" "+pretty_bool_exp+ 
        '<'+str_jline.str()+'>');
}
bool IfThen::getBooleanValue(std::map <std::string, int>& var,
    std::map<std::string, std::map <int, int> >& array) const
{
    bool true_or_false = boolean->getValue(var, array);
    return true_or_false;
}
int IfThen::getJline() const
{
    return jump;
}
IfThen::~IfThen()
{
   delete this->boolean;
}
GoTo::GoTo(int line_num, int& jline)
{
    line_number=line_num;
    jump2 = jline;
}
string GoTo::getType()const
{
    return ("GOTO");
}
string GoTo::format() const
{
    stringstream str_line; //convert the line numbers back to strings
    str_line<<line_number;

    stringstream jump_line;
    jump_line<<jump2;

    string full = str_line.str()+ " GOTO <"+jump_line.str()+">";
    return (full);
}
int GoTo::getJline() const
{
    return jump2;
}
GoTo::~GoTo(){}

GoSub::GoSub(int line_num, int& jline)
{
    line_number = line_num;
    jump_to = jline;
}
string GoSub::getType()const
{
    return ("GOSUB");
}
string GoSub::format() const
{
    stringstream str_line; //convert line numbers back to strings
    str_line<<line_number;

    stringstream jump_line;
    jump_line<<jump_to;
    string full = str_line.str()+" GOSUB <"+jump_line.str()+">";
    return(full);
}

int GoSub::getJline() const
{
    return jump_to;
}
GoSub::~GoSub(){}

Return::Return(int line_num)
{
    line_number = line_num;
}
string Return::getType()const
{
    return ("RETURN");
}
string Return::format() const
{
    stringstream str_line;  //converts line number back to string
    str_line<<line_number;

    return (str_line.str()+ " RETURN");
}

Return::~Return(){}

End::End(int line_num)
{
    line_number = line_num;
}
string End::getType()const
{
    return ("END");
}
string End::format() const
{
    stringstream str_line; //converts line number back to string
    str_line<<line_number;

    return(str_line.str()+ " END");
}
End::~End(){}