#include "interpreter.h"
#include "command.h"
#include "arithmetic.h"
#include <string>
#include <map>
#include <stdexcept>
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
Interpreter::Interpreter(std::istream& in) {
    this->parse(in);
}

Interpreter::~Interpreter() {
    map<int, Command*>:: iterator it;
    for (it=commands.begin(); it != commands.end(); it++)
    {
        delete (it->second);
    }
    commands.clear();
}
NumericExpression* Interpreter::parse_const(string& express, int& pos)
{
        NumericExpression* constant; //stores pointer to constant class
        int val;    
        string dig="";

        //if first char is negative, then append it to dig
        //and increment position
        if (express[pos]=='-')
        {
            dig+=express[pos];
            ++pos;
        }
        while(isdigit(express[pos]))
        {
            dig += express[pos];
            ++pos;
        }
        //convert to int
        stringstream ss(dig);
        ss>>val;    
        constant = new Constant (val);
        return constant;
} 
NumericExpression* Interpreter::parse_variable(string& express, int& pos)
{
    string name = "";
    //used to test if we have an array
    int open_bracket_index=-1;
    
    int start_pos = pos;

    //loop thru and store name until we hit a bracket or non letter
    for (int i=start_pos; i<(int)express.size(); i++)
    {
        if (express[i]=='[')
        {
            open_bracket_index = i+1; //store index of the start of expression in bracket
            break;
        }
        if (!isalpha(express[i]))
        {
            break;
        }
        name+=express[i];
        ++pos;
    }
    if (open_bracket_index==-1)
    {
        //if we just a variable name, we store it
        NumericExpression* variable;
        variable = new normalVar(name);
        return variable;
    }
    else //if open bracket found, call parse_expression again & store array
    { 
        pos = open_bracket_index; //want to keep incr pos while parsing index
        NumericExpression* array_index_expression =
        parse_expression(express, pos, NULL);

        NumericExpression* arr = new arrayVar (name, array_index_expression);
        ++pos; //increment one more time so we are at '[' or past it
        return arr;
    }
}   
NumericExpression* Interpreter::parse_expression (string& express, int& pos, NumericExpression* left)
{
    NumericExpression* expression;
    NumericExpression* right;

    if (isdigit (express[pos]))
    {
        //parse the constant and store it
        expression = parse_const (express, pos);
        return expression;
    }
    else if (express[pos]== '-' && pos == 0 && isdigit(express[pos+1]))
    {
        expression = parse_const(express, pos);
        return expression;
    }
    //check if we for sure have a negative integer & store it
    else if (express[pos]=='-'&& isdigit(express[pos+1]))
    {
        if (express[pos-1]=='(' || express[pos-1]=='+'||
            express[pos-1]=='-'||express[pos-1]=='*'||
            express[pos-1]=='/'||express[pos-1]=='[')
        {
            //parse the negative constant and store it
            expression=parse_const(express, pos);
            return expression;
        }
    }

    //check if the current position is a letter & parse accordingly
    //and store it
    else if  (isalpha(express[pos]))
    {
        expression = parse_variable(express, pos);
        return expression;
        
    }
    if (express[pos]=='(')
    {
        left = parse_expression(express, ++pos, left);
    }
    
    //if we find a closing parentheses, but the position
    //hasn't reached the end, keep parsing
    if (express[pos]==')'&& pos!=express.size()-1)
    {
        expression = parse_expression(express, ++pos, left);
    }
    if(express[pos]==']'&& pos!=express.size()-1)
    {
        ++pos;
    }
    //if an operator is found at the current position
    //we store it and parse the left & right expressions accordingly
    if (express[pos]=='+'||express[pos]=='-'
    ||express[pos]=='*'||express[pos]=='/')
    {
        char op = express[pos];
        right = parse_expression(express, ++pos, left);
        if (op=='+')
        {
            expression = new AdditionExpression(left, right);
        }
        else if (op=='-')
        {
            expression = new SubtractionExpression(left, right);
        }
        else if (op=='*')
        {
            expression = new MultiplicationExpression(left, right);
        }
        else if (op == '/')
        {
            expression = new DivisionExpression(left, right);
        }
    }

    return expression;
}

void Interpreter::remove_ws(stringstream& stream, string& exp)
{
    string remaining; //buffer variable to continue streaming
    while (stream>>remaining)
    {
        exp+=remaining;
    }
}

void Interpreter::parse(istream& in) {
    string line;

    while (getline(in, line)) {
        size_t line_numero;
        stringstream stream(line);
        stream >> line_numero;

        string command;
        stream >> command;

        if(command == "PRINT")
        {
            string exp="";
            //gets the full expression w/out whitespace
            remove_ws(stream, exp); 

           //parse expression correctly
            int position=0;
            NumericExpression* pretty_exp = 
            parse_expression(exp, position, NULL);

            commands[line_numero]=(new Print(line_numero, pretty_exp));
           
        }   
       else if (command == "LET")
        {
            string temp_name="";
            string exp="";
            string name="";

            stream>>temp_name; //takes in everything until whitespace is hit
            int i=0;

            //stores alphanumeric chars from temp_name into name (will deal with [ later)
            while(isalpha(temp_name[i]))  
            {
                name+=temp_name[i];
                i++;
            }
            exp = temp_name.substr(i); //appends all the reamining chars from temp_name into exp

            //checks if what we have next is another variable (Y or Y[12]) & exp is does not have an index exp 
            if(isalpha(stream.peek()) && exp[0]!='[') 
            {
                remove_ws(stream, exp);  
                int pos=0;
                int posit=0;
                NumericExpression* val = parse_expression(exp, posit, NULL);

                //create new variable object with name & parsed variable 
                //for when Let object is created
                NumericExpression* variable = parse_expression(name, pos, NULL);
                commands[line_numero]=(new Let(line_numero, variable, val));
            }
            else
            {
                remove_ws(stream, exp); //append the rest of the line (if any) to exp
                if (exp[0]=='[') //check to see if we have an array var & parse index
                {
                    int open_bracket_index;
                    for (int j=0; j<(int)exp.size(); j++)
                    {
                        if (exp[j]==']'&& exp[j+1]!=']'&&
                        (isdigit(exp[j+1])||isalpha(exp[j+1])
                        || exp[j+1] == '('))
                        {
                            name+=exp[j];
                            open_bracket_index=j;
                            break;
                        }
                        name+=exp[j];
                    }
                    //erase index from the expression
                    exp.erase(0, open_bracket_index+1);
                }
                int position=0;
                NumericExpression* pretty_exp = 
                parse_expression(exp, position, NULL);
                   
                //create new variable object with name & parsed expression 
                //for when Let object is created
                int posicion=0;
                NumericExpression* variable = parse_expression(name, posicion, NULL);
                commands[line_numero]=(new Let(line_numero, variable, pretty_exp));
            }
           
        }
        else if(command == "GOTO")
        {
            int jline;
            stream >> jline;
            commands[line_numero]=(new GoTo(line_numero, jline));
        } 
        else if (command == "IF")
        {
            string left= "";
            string right = "";
            char bool_op;
            string full_exp= "";
            string jline="";

            remove_ws(stream, full_exp);//removes all whitespace following IF

            int index_where_then_starts;

            string comparisons=""; //variable to store boolean expression

            for (int i=0; i<(int)full_exp.size(); i++)
            {
                //store where THEN begins
                if (full_exp[i] == 'T' && full_exp[i+1] =='H'
                    && full_exp[i+2]=='E'&& full_exp[i+3]=='N')
                {
                    index_where_then_starts=i;
                    break;
                }
            }
            //store the boolean expressions
            comparisons = full_exp.substr (0, index_where_then_starts);

            //store the line to jump to in a string
            for (int j=index_where_then_starts+4; 
                j<(int)full_exp.size(); j++)
            {
                 jline += full_exp[j];
            }
           
            full_exp.clear(); //get rid of the full expression

            int index_where_right_starts;

            //store the left side of the boolean expression
            //and store the operator & 
            //index where right side starts 
            //once we hit the operator
            for(int k=0; k<(int)comparisons.size(); k++)
            {
                if (comparisons[k]=='>'||comparisons[k]=='<'||
                    comparisons[k]== '=')
                {
                    bool_op = comparisons[k];
                    index_where_right_starts= k+1;
                    break;
                }
                left+=comparisons[k];
            }

            //store the right side of the boolean expression
            for(int m=index_where_right_starts; 
                m<(int)comparisons.size(); m++)
            {
                right+=comparisons[m];
            }

            comparisons.clear(); //get rid of the bool expression
            int position_l=0;
            int position_r=0;

            //parse left and right side
            NumericExpression* pretty_left = 
            parse_expression(left, position_l, NULL);
        
            NumericExpression* pretty_right=
            parse_expression(right, position_r, NULL);

            //create boolean expression pointer to pass it in and use in IfThen class
            booleanExpression* b = new booleanExpression(pretty_left, bool_op, pretty_right);
            commands[line_numero]=(new IfThen(line_numero, b, jline));
           
        }
        else if (command == "GOSUB")
        {
            int jline;
            stream >> jline;
            GoSub gsub(line_numero, jline);
            commands[line_numero]=(new GoSub(line_numero, jline));
        }
        else if (command == "RETURN")
        {
            commands[line_numero]=(new Return(line_numero));
        }
        else if (command == "END")
        {
            commands[line_numero]=(new End(line_numero));
        }
    }

}

void Interpreter::run(std::ostream& out) {

    map <int, Command*>::iterator it=commands.begin();
    stack <int> return_to; //stack for GOSUB and RETURN
    while(it != commands.end())
    {
        try
        {
            if (it->second->getType()=="PRINT")
            {
                Print* p = dynamic_cast<Print* > (it->second);
                p->getExpression(var, array); //try to get val of expression first & then print if no errors
                out << p->getExpression(var, array) << '\n';
                it++;
            }
            else if (it->second->getType()=="LET")
            {
                Let* l = dynamic_cast<Let* > (it->second);
                l->setValue(var, array);
                it++;
            }
            else if (it->second->getType()=="IF")
            {
                IfThen* ifth = dynamic_cast <IfThen* > (it->second);
                if(ifth->getBooleanValue(var,array))
                {

                    if(commands.find(ifth->getJline()) != commands.end())
                    {
                        it = commands.find(ifth->getJline());
                    }
                    else
                    {
                        stringstream jline;
                        jline << ifth->getJline();
                        string error = "IF jump to non-existent line " + jline.str();
                        throw invalid_argument(error);
                    }
                }
                else
                {
                    it++; //if the expression didn't evaluate to true, go to the following command
                }
            }
            else if(it->second->getType()=="GOTO")
            {
                GoTo* go2 = dynamic_cast <GoTo* > (it->second);
           
                if(commands.find(go2->getJline())!=commands.end())
                {
                    it = commands.find(go2->getJline());
                }
                else
                {
                    stringstream jline;
                    jline << go2->getJline();
                    string error = "GOTO jump to non-existent line " + jline.str();
                    throw invalid_argument(error);
                }
            }
            else if (it->second->getType()=="GOSUB")
            {
                GoSub* gsub = dynamic_cast <GoSub* > (it->second);
                if(commands.find(gsub->getJline())!=commands.end())
                {
                    return_to.push(it->first);
                    it = commands.find(gsub->getJline());
                }
                else
                {
                    stringstream jline;
                    jline << gsub->getJline();
                    string error = "GOSUB jump to non-existent line " + jline.str();
                    throw invalid_argument(error);
                }

            }
            else if (it->second->getType()=="RETURN")
            {
                Return* r = dynamic_cast<Return* > (it->second);
                if(!return_to.empty())
                {
                    it = commands.find(return_to.top());
                    return_to.pop();
                    ++it;
                }
                else
                {
                    throw invalid_argument("No matching GOSUB for RETURN");
                }
            }
            else if (it->second->getType()=="END")
            {
                End* end = dynamic_cast <End* > (it->second);
                break; 
            }
        }
        catch (exception &e)
        {
            out << "Error in Line " << (it)->first << ": " << e.what()<<"." << '\n';
            break;
        }    
    }  
}
