#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP
#include <string>
#include <map>

class NumericExpression {
    public:
        virtual ~NumericExpression() {}
        virtual std::string format() const = 0;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const=0;
};
class Constant: public NumericExpression
{
    public:
        Constant (int& val);
        ~Constant();
        virtual std::string format()const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;
    private:
        int const_c;
};
class normalVar: public NumericExpression
{
    public:
        normalVar(std::string& name);
        ~normalVar();
        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;

        void setValue(int v, std::map <std::string, int>& var);
        
    private:
        std::string nombre;
        int value;    
};
class arrayVar: public NumericExpression
{
    public:
        arrayVar (std::string& name, NumericExpression* index);
        ~arrayVar ();
        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;

        void setValue(int v, std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array);
    private:
        std::string array_name;
        NumericExpression* index;  
        int value;
        int index_val; 
};

class AdditionExpression : public NumericExpression 
{
    public:
        AdditionExpression(NumericExpression* left, NumericExpression* right);
        ~AdditionExpression();

        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;
    private:
        NumericExpression* left;
        NumericExpression* right;
};

class SubtractionExpression : public NumericExpression 
{
    public:
        SubtractionExpression(NumericExpression* left, NumericExpression* right);
        ~SubtractionExpression();

        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;
    private:
        NumericExpression* left;
        NumericExpression* right;
};

class DivisionExpression : public NumericExpression 
{
    public:
        DivisionExpression(NumericExpression* left, NumericExpression* right);
        ~DivisionExpression();

        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;

        std::string error_format(std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array)const;
    private:
        NumericExpression* left;
        NumericExpression* right;
        int l;
        int r;
};

class MultiplicationExpression : public NumericExpression 
{
    public:
        MultiplicationExpression(NumericExpression* left, NumericExpression* right);
        ~MultiplicationExpression();

        virtual std::string format() const;
        virtual int getValue (std::map <std::string, int>& var,
        std::map<std::string, std::map <int, int> >& array) const;
    private:
        NumericExpression* left;
        NumericExpression* right;
};


#endif