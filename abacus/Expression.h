#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>
#include <string>
#include <stack>
#include <map>
#include "MathEx.h"
#include "metacharacter.h"

using namespace std;

namespace ExpressionError
{
    const string ILLEGAL_CHARACTER_ERROR = "非法字符: ";
    const string ILLEGAL_BRACKET_ERROR = "括号不匹配";
    const string MISSING_OPERAND_ERROR = "缺少操作数";
    const string MISSING_OPERATOR_ERROR = "缺少运算符";
    const string PERCENT_OPERATOR_ERROR = "百分号前只能为数字或括号包围的表达式";
    const string DEGREE_OPERATOR_ERROR = "角度符号前只能为数字或括号包围的表达式";
    const string SQUARE_ROOT_ERROR = "开平方运算需要用括号包围";
    const string UNKNOWN_ERROR = "未知错误";
}

class Expression
{

public:
    Expression(string str, int precision = 5);
	~Expression();
	double getResult();

private:
	MathEx mathEx;
	string raw_exp;
    list<Metacharacter> exp;
    stack<Metacharacter> op;
	stack<double> number;

	double result = 0;

	bool simpleCheck();
	bool split();
	void negativeOperatorPreprocessing();
	void sqrtOperatorPreprocessing();
	void percentOperatorPreprocessing();
    void degreeOperatorPreprocessing();
	void bracketPreprocessing();
	void preprocessing();
	void operation();
	void transToPostfix();
    void calc(Metacharacter mc, double &op1);
    void calc(Metacharacter mc, double &op1, double &op2);
};


#endif
