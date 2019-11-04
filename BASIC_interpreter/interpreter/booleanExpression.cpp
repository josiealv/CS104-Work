#include "booleanExpression.h"
#include "arithmetic.h"
#include <map>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;
booleanExpression::booleanExpression(NumericExpression* left, char op, 
NumericExpression* right)
{
	left_bool = left->format(); //store the left format expression locally
	right_bool = right->format(); //store the right format expression locally
	operate =op; //store the boolean operator locally 


	if (operate == '>') //if we get > , we flip the expressions & change to <
	{
		string temp = left_bool;
		left_bool = right_bool;
		right_bool = temp;

		NumericExpression* temp_exp = left;
		l = right;
		r = left;
		
		operate = '<';
	}
	else
	{
		l = left;
		r = right;
	}

}
bool booleanExpression::getValue(map<string, int>& var, 
map<string, map<int, int> >& array) const
{
	if (operate == '<')
	{
		if (l->getValue(var, array) < r->getValue(var, array))
		{
			return true;
		}
		return false;
	}
	else if (operate == '=')
	{
		if (l->getValue(var, array) == r->getValue(var, array))
		{
			return true;
		}
		return false;
	}
}
booleanExpression::~booleanExpression()
{
	delete this->l;
	delete this->r;
}
string booleanExpression::format() const
{
	return ("IF ["+ left_bool+" " + operate +" " + right_bool+
	"]"+ " THEN ");
}