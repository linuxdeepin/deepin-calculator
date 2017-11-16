#include "Expression.h"
#include <exception>
#include <regex>
#include <cctype>
#include <cmath>
#include <iostream>

using namespace std;

Expression::Expression(string str, int precision)
{
    raw_exp = str;
    mathEx.set_eps(precision);
}

Expression::~Expression()
{
}

bool Expression::simpleCheck()
{
    string temp;
    bool getWord = false;
    int isAlpha = 0;
    for (auto &i : raw_exp)
    {
        isAlpha = isalpha(i);
        if (getWord)
        {
            if (isAlpha)
            {
                temp += i;
                continue;
            }
        }
        else
        {
            if (isAlpha)
            {
                getWord = true;
                temp += i;
                continue;
            }
            else
                temp += i;
        }

        if (METACHARACTERS.find(temp) == METACHARACTERS.end() || temp == "$")
            throw runtime_error(ExpressionError::ILLEGAL_CHARACTER_ERROR + temp);

        if (getWord)
        {
            getWord = false;
        }
        temp = "";
    }

    if (getWord)
		throw runtime_error(ExpressionError::ILLEGAL_BRACKET_ERROR);
	return true;
}

bool Expression::split()
{
	string::size_type i = 0;
	string::size_type temp = 0;
	while (i<raw_exp.size())
	{
            string str_temp;
            str_temp += raw_exp[i];
            bool getPoint = false;

            if (isdigit(raw_exp[i]))
            {
                temp = i;
                do
                {
                    if(raw_exp[i] == '.')
                        getPoint = true;
                    if (++i >= raw_exp.size())
                        break;
                } while (isdigit(raw_exp[i]) || ((!getPoint) && raw_exp[i] == '.'));
                exp.push_back(Metacharacter{ 0,0,0,0,0, raw_exp.substr(temp, i - temp) });
            }
        else if (raw_exp[i] == '.')
			throw runtime_error(ExpressionError::ILLEGAL_CHARACTER_ERROR + ".");
        else if (METACHARACTERS.find(str_temp)!=METACHARACTERS.end())
        {
            exp.push_back(METACHARACTERS.at(str_temp));
            ++i;
        }
        else if (isalpha(raw_exp[i]))
        {
            temp = i;
            do
            {
                if (++i >= raw_exp.size())
                    break;
            } while (isalpha(raw_exp[i]));
            str_temp = raw_exp.substr(temp, i - temp);
            exp.push_back(METACHARACTERS.at(str_temp));
        }
	}

	return true;
}

void Expression::negativeOperatorPreprocessing()
{
    for (list<Metacharacter>::iterator i = exp.begin(); i != exp.end(); ++i)
    {
        if ((*i).e == "-")
        {
            if (i == exp.begin())
            {
                i = exp.insert(i, METACHARACTERS.at("0"));
            }
            else
            {
                --i;
                if ((*i).out_priority >= 100)
                {
                    ++i;
                    i = exp.insert(i, METACHARACTERS.at("0"));
                }
                ++i;
            }
        }
	}
}

void Expression::sqrtOperatorPreprocessing()
{
    for (list<Metacharacter>::iterator i = exp.begin(); i != exp.end(); ++i)
    {
        if ((*i).e == "#")
        {
            if (i == exp.begin())
            {
                *i = METACHARACTERS.at("<");
            }
            else
            {
                --i;
                if ((*i).out_priority >= 100 || (*i).position == 1 || (*i).position == 2)
                {
                    if(i->type == 1 && i->in_priority >= METACHARACTERS.at("<").in_priority)
                        throw runtime_error(ExpressionError::SQUARE_ROOT_ERROR);
                    ++i;
                    *i = METACHARACTERS.at("<");
                }
                else
                    ++i;
            }
        }
	}
}

void Expression::percentOperatorPreprocessing()
{
    for (list<Metacharacter>::iterator i = exp.begin(); i != exp.end(); ++i)
    {
        if ((*i).e == "%")
        {
            ++i;
            if (i == exp.end() || (*i).position == 2 || (*i).position == 3)
            {
                --i;--i;
                if(i->type!=0 && i->in_priority<=100)
                    throw runtime_error(ExpressionError::PERCENT_OPERATOR_ERROR);
                ++i;
                *i = METACHARACTERS.at(">");
            }
        }
    }
}

void Expression::degreeOperatorPreprocessing()
{
    for (list<Metacharacter>::iterator i = exp.begin(); i != exp.end(); ++i)
    {
        if ((*i).e == "`")
        {
            --i;
            if(i->type!=0 && i->in_priority<=100)
                throw runtime_error(ExpressionError::DEGREE_OPERATOR_ERROR);
            ++i;
        }
    }
}

void Expression::bracketPreprocessing()
{
    for (list<Metacharacter>::iterator i = exp.begin(); i != exp.end(); ++i)
    {
        if ((*i).out_priority ==101 || (*i).out_priority == 102)
        {
            i = exp.erase(i);
            i = exp.insert(i, METACHARACTERS.at("("));
        }
        else if ((*i).in_priority == 101 || (*i).in_priority == 102)
		{
			i = exp.erase(i);
            i = exp.insert(i, METACHARACTERS.at(")"));
		}
	}
}

void Expression::preprocessing()
{
    negativeOperatorPreprocessing();
    sqrtOperatorPreprocessing();
    percentOperatorPreprocessing();
    bracketPreprocessing();
    degreeOperatorPreprocessing();
}

void Expression::operation()
{
    if (op.top().operand == 1)  //若为一元运算符
    {
        double op1;
        if (op.empty())
            throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
        else
            calc(op.top(), op1);
    }
    else    //若为二元运算符
    {
        double op1, op2;
        if (op.empty())
            throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
        else
            calc(op.top(), op1, op2);
    }
    op.pop();
}


void Expression::transToPostfix()
{
	try {
        exp.push_front(METACHARACTERS.at("$"));
		for (auto i : exp)
		{
			if (i.type == 0)
				number.push(stod(i.e));

			else if (i.type == 1)
			{
				if (i.out_priority <= op.top().in_priority)
				{
					do
					{
						operation();
					} while (i.out_priority <= op.top().in_priority);
				}
				op.push(i);
			}

			else if (i.type == 2)
			{
				//如果当前括号为左括号，入栈
				if (i.out_priority > 0)
				{
					op.push(i);
					//lastBracket = i;
				}
				else if (i.out_priority < 0)
				{
                    while (op.top().type != 2)
					{
						operation();
                    }

					op.pop();
					if (op.empty())
						throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
				}
			}
			else if (i.type == 3)
				op.push(i);
		}

		while (op.size() > 1)
		{
			operation();
		}

        if(number.size() > 1)
        {
            throw runtime_error(ExpressionError::MISSING_OPERATOR_ERROR);
        }
	}
	catch (...) {
		throw;
	}
}

void Expression::calc(Metacharacter mc, double & op1)
{
	if (number.empty())
		throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
	else
	{
		op1 = number.top();
		number.pop();
	}
	try {
		if (mc.e == "!")
			number.push(mathEx.op_factorial(op1));
        else if (mc.e == "`")
            number.push(mathEx.op_degree_to_radian(op1));
        else if (mc.e == ">")
            number.push(mathEx.op_percent(op1));
        else if (mc.e == "<")
            number.push(mathEx.op_square_root(op1));
		else if (mc.e == "sin")
			number.push(mathEx.op_sin(op1));
		else if (mc.e == "cos")
			number.push(mathEx.op_cos(op1));
		else if (mc.e == "tan")
			number.push(mathEx.op_tan(op1));
		else if (mc.e == "arcsin")
			number.push(mathEx.op_arcsin(op1));
		else if (mc.e == "arccos")
			number.push(mathEx.op_arccos(op1));
		else if (mc.e == "arctan")
			number.push(mathEx.op_arctan(op1));
		else if (mc.e == "log" || mc.e == "lg")
			number.push(mathEx.op_lg(op1));
		else if (mc.e == "ln")
			number.push(mathEx.op_ln(op1));
		else
			throw runtime_error(ExpressionError::ILLEGAL_CHARACTER_ERROR + mc.e);
	}
	catch (runtime_error) {
		throw;
	}

}

void Expression::calc(Metacharacter mc, double & op1, double & op2)
{
    if (number.empty())
		throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
	else
	{
		op2 = number.top();
		number.pop();
	}
    if (number.empty())
		throw runtime_error(ExpressionError::MISSING_OPERAND_ERROR);
	else
	{
		op1 = number.top();
		number.pop();
	}
	try {
		if (mc.e == "+")
			number.push(mathEx.op_add(op1, op2));
		else if (mc.e == "-")
			number.push(mathEx.op_subtract(op1, op2));
		else if (mc.e == "*")
			number.push(mathEx.op_multiply(op1, op2));
		else if (mc.e == "/")
			number.push(mathEx.op_divide(op1, op2));
		else if (mc.e == "^")
			number.push(mathEx.op_pow(op1, op2));
		else if (mc.e == "%")
			number.push(mathEx.op_mod(op1, op2));
		else if (mc.e == "#")
			number.push(mathEx.op_extract_root(op1, op2));
		else
			throw runtime_error(ExpressionError::ILLEGAL_CHARACTER_ERROR + mc.e);
	}
	catch (runtime_error) {
		throw;
	}
}

double Expression::getResult()
{
	try {
        simpleCheck();
        split();
        preprocessing();
        transToPostfix();
    } catch (runtime_error &){
		throw;
	}
    result = number.top();
    //return to_string(mathEx.getRound(result));
    return result;
}


