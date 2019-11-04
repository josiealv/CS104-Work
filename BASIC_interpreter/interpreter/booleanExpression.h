#ifndef BOOLEANEXPRESSION_HPP
#define BOOLEANEXPRESSION_HPP
#include "arithmetic.h"
#include <map>
#include <string>

class booleanExpression 
{
	public:
		booleanExpression(NumericExpression* left, char op, NumericExpression* right);
		std::string format() const;
		~booleanExpression();
		bool getValue(std::map <std::string, int>& var,
    std::map<std::string, std::map <int, int> >& array) const;

	private:
		NumericExpression* l;
		NumericExpression* r;
		std::string left_bool;
		std::string right_bool;
		char operate;	
};

#endif
