#include "arithmetic.h"
#include "command.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
using namespace std;

Constant::Constant(int& val)
{
    const_c=val; //store value locally
}
Constant::~Constant()
{}
int Constant::getValue(map<string, int>& var, 
map<string, map<int, int> >& array)const
{
    return (const_c);
}
string Constant::format()const
{
    stringstream character; //convert constant back to a character
    character<<const_c;
    return character.str();
}
normalVar::normalVar(string& name): nombre(name)
{
   value = 0; //initialize value as 0 first because the constructor is only called when we parse
}
string normalVar::format() const
{
    return nombre; 
}
int normalVar::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    //if the value isn't in the map, insert it with the current value of the object
    if (var.find(nombre)==var.end())
    {
        var[nombre]=value;
    }
    return var[nombre];
}
void normalVar::setValue(int v, map<string, int>& var)
{
    value = v;
    var[nombre] = value;
}
normalVar::~normalVar()
{}
arrayVar::arrayVar (std::string& name, NumericExpression* index):
array_name(name), index(index) //index is set equal to index that is passed in
{
    value = 0; //initialize value as 0 first because the constructor is only called when we parse
}
string arrayVar::format() const
{
    return (array_name+'['+ this->index->format()+']');
}
int arrayVar::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    int index_val = index->getValue(var, array);

    //if the value isn't in the map, insert it with the current value of the object
    if (array.find(array_name)==array.end()||
        array[array_name].find(index_val)==array[array_name].end())
        {
            array[array_name][index_val] = value;
        }

    return array[array_name][index_val];
}
void arrayVar::setValue(int v, map<string, int>& var,
map<string, map<int, int> >& array)
{
    value = v; 
    index_val = index->getValue(var, array);
    array[array_name][index_val] = value;
}
arrayVar::~arrayVar()
{
    delete this->index;
}

AdditionExpression::AdditionExpression
(NumericExpression* left, NumericExpression* right)
 : left(left), right(right) {

}
int AdditionExpression::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    return (left->getValue(var, array)+right->getValue(var, array));
}
AdditionExpression::~AdditionExpression() {
    delete this->left;
    delete this->right;
}

string AdditionExpression::format() const {
    return "(" + this->left->format() + " + " + this->right->format() + ")";
}
SubtractionExpression::SubtractionExpression
(NumericExpression* left, NumericExpression* right)
 : left(left), right(right) {
}
int SubtractionExpression::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    return (left->getValue(var, array)-right->getValue(var, array));
}
SubtractionExpression::~SubtractionExpression() {
    delete this->left;
    delete this->right;
}

string SubtractionExpression::format() const {
    return "(" + this->left->format() + " - " + this->right->format() + ")";
}
DivisionExpression::DivisionExpression
(NumericExpression* left, NumericExpression* right)
 : left(left), right(right) {

}
int DivisionExpression::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    if (right->getValue(var, array)==0) //if trying to divide by 0, throw error with correct error format
    {
        throw invalid_argument(error_format(var, array));
    }
    return (left->getValue(var, array)/right->getValue(var, array));
}
string DivisionExpression::error_format(map<string, int>& var, 
map<string, map<int, int> >& array)const
{
    stringstream sr;
    sr << right->getValue(var, array);

    stringstream sl;
    sl << left->getValue(var, array);
    
    //format for the error message
    string error = "Division by 0: " +left->format()+ " = " + sl.str() 
    + ", "+ right->format()+" = "+ sr.str();
    return (error);
}
DivisionExpression::~DivisionExpression() {
    delete this->left;
    delete this->right;
}

string DivisionExpression::format() const {
    return "(" + this->left->format() + " / " + this->right->format() + ")";
}

MultiplicationExpression::MultiplicationExpression
(NumericExpression* left, NumericExpression* right)
 : left(left), right(right) {

}
int MultiplicationExpression::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
    return (left->getValue(var, array)*right->getValue(var, array));
}
MultiplicationExpression::~MultiplicationExpression() {
    delete this->left;
    delete this->right;
}

string MultiplicationExpression::format() const {
    return "(" + this->left->format() + " * " + this->right->format() + ")";
}
