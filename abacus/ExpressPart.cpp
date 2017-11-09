#include <stdlib.h>
#include "ExpressPart.h"

namespace zhcosin
{
using namespace std;

extern plus_functor plusor;
extern minus_functor minusor;
extern negate_functor negator;
extern multiplies_functor multiplor;
extern divides_functor dividor;
extern modulus_functor modulor;
extern abs_functor absor;
extern pow_functor powor;
extern exp_functor expor;
extern log_functor logor;
extern ln_functor lnor;
extern sqrt_functor sqrtor;
extern sin_functor sinor;
extern cos_functor cosor;
extern tan_functor tanor;
extern arcsin_functor arcsinor;
extern arccos_functor arccosor;
extern arctan_functor arctanor;
extern factorial_functor factorialor;
extern double_factorial_functor dfactorialor;
extern cb_functor cbor;
extern max_functor maxor;
extern min_functor minor;
extern ceil_functor ceilor;
extern floor_functor flooror;
extern sinh_functor sinhor;
extern cosh_functor coshor;
extern tanh_functor tanhor;

int ExpressPart::DestroyVectorOfExpressPartPtr(list<ExpressPart*> &container)
{
	if(container.empty()) return 0;

	for(list<ExpressPart*>::iterator iter=container.begin(); iter!=container.end(); iter++)
	{
		delete *iter;
		*iter=NULL;
	}
	container.clear();

	return 0;
}

vector<ExpOperator*>	ExpOperator::OperatorList;
int		ExpOperator::AnyNumbers = 100;

ExpOperator::ExpOperator()
{
	nNumber		= 0;
	priority	= 0;
	functor		= NULL;
}

ExpOperator::ExpOperator(const string &opt) : ExpressPart(opt)
{
	nNumber		= 0;
	priority	= 0;
	functor		= NULL;

	if(!IsInitOperatorList()) InitOperatorList();

	for(vector<ExpOperator*>::const_iterator iter = OperatorList.begin(); iter != OperatorList.end(); iter++)
	{
		if(opt == (*iter)->ReturnStrBody())
		{
			nNumber		= (*iter)->nNumber;
			priority	= (*iter)->priority;
			functor		= (*iter)->functor;
			break;
		}
	}

}

bool ExpOperator::IsInitOperatorList()
{
	return !OperatorList.empty();
}

ExpOperator::ExpOperator(const string &opt,const int nNumber,const int priority,operator_functor *functor): ExpressPart(opt/*,expPartTypeOperator*/)
{
	this->nNumber	= nNumber;
	this->priority	= priority;
	this->functor	= functor;
}

bool ExpOperator::IsLegal()const
{
	if(0==nNumber) 		return false;
	if(0==priority) 	return false;
	if(NULL==functor) 	return false;

	return true;
}

bool ExpOperator::InitOperatorList()
{
	if(IsInitOperatorList()) return 0;

    OperatorList.push_back(new ExpOperator("+", 2, 1, (operator_functor*)&plusor));
	OperatorList.push_back(new ExpOperator("-", 2, 1, (operator_functor*)&minusor));
	OperatorList.push_back(new ExpOperator("-", 1, 2, (operator_functor*)&negator));
	OperatorList.push_back(new ExpOperator("*", 2, 2, (operator_functor*)&multiplor));
	OperatorList.push_back(new ExpOperator("/", 2, 2, (operator_functor*)&dividor));
	OperatorList.push_back(new ExpOperator("%", 2, 2, (operator_functor*)&modulor));
	OperatorList.push_back(new ExpOperator("abs", 1, 100, (operator_functor*)&absor));
	OperatorList.push_back(new ExpOperator("pow", 2, 100, (operator_functor*)&powor));
	OperatorList.push_back(new ExpOperator("^", 2, 3, (operator_functor*)&powor));
	OperatorList.push_back(new ExpOperator("exp", 1, 100, (operator_functor*)&expor));
	OperatorList.push_back(new ExpOperator("log", 2, 100, (operator_functor*)&logor));
	OperatorList.push_back(new ExpOperator("ln", 1, 100, (operator_functor*)&lnor));
	OperatorList.push_back(new ExpOperator("sqrt", 1, 100, (operator_functor*)&sqrtor));
	OperatorList.push_back(new ExpOperator("sin", 1, 100, (operator_functor*)&sinor));
	OperatorList.push_back(new ExpOperator("cos", 1, 100, (operator_functor*)&cosor));
	OperatorList.push_back(new ExpOperator("tan", 1, 100, (operator_functor*)&tanor));
	OperatorList.push_back(new ExpOperator("arcsin", 1, 100, (operator_functor*)&arcsinor));
	OperatorList.push_back(new ExpOperator("arccos", 1, 100, (operator_functor*)&arccosor));
	OperatorList.push_back(new ExpOperator("arctan", 1, 100, (operator_functor*)&arctanor));
	OperatorList.push_back(new ExpOperator("factorial", 1, 100, (operator_functor*)&factorialor));
	OperatorList.push_back(new ExpOperator("!", 1, 3, (operator_functor*)&factorialor));
	OperatorList.push_back(new ExpOperator("!!", 1, 100, (operator_functor*)&dfactorialor));
	OperatorList.push_back(new ExpOperator("cb", 2, 100, (operator_functor*)&cbor));
	OperatorList.push_back(new ExpOperator("max", AnyNumbers, 100, (operator_functor*)&maxor));
	OperatorList.push_back(new ExpOperator("min", AnyNumbers, 100, (operator_functor*)&minor));
	OperatorList.push_back(new ExpOperator("ceil", 1, 100, (operator_functor*)&ceilor));
	OperatorList.push_back(new ExpOperator("floor", 1, 100, (operator_functor*)&flooror));
	OperatorList.push_back(new ExpOperator("sinh", 1, 100, (operator_functor*)&sinhor));
	OperatorList.push_back(new ExpOperator("cosh", 1, 100, (operator_functor*)&coshor));
	OperatorList.push_back(new ExpOperator("tanh", 1, 100, (operator_functor*)&tanhor));

	return true;
}

bool ExpOperator::UnInitOperatorList()
{
	for (vector<ExpOperator*>::iterator it = OperatorList.begin(); it != OperatorList.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	OperatorList.clear();

	return true;
}

bool ExpOperator::IsOperator(const string &str4Parse)
{
	if(!IsInitOperatorList()) InitOperatorList();

	for (vector<ExpOperator*>::const_iterator it = OperatorList.begin(); it != OperatorList.end(); it++)
	{
		if(str4Parse == (*it)->ReturnStrBody()) return true;
	}

	return false;
}

bool ExpOperator::IsCommonOperator()const
{
	if(!IsLegal()) return false;

	return !ExpIdentifier::IsIdentifier(strBody);
}

bool ExpOperator::IsFunctionOperator()const
{
	if(!IsLegal()) return false;

	return ExpIdentifier::IsIdentifier(strBody);
}

vector<ExpNumber*>	ExpNumber::SymbConstList;

ExpNumber::ExpNumber(const string &str):ExpressPart(str)
{
	value=0;
	hasValidValue = (GenerateValue() == 0);
}

bool ExpNumber::IsInitSymbConstList()
{
	return !SymbConstList.empty();
}

void ExpNumber::InitSymbConstList()
{
	SymbConstList.push_back(new ExpNumber("pi",3.1415926535));
	SymbConstList.push_back(new ExpNumber("e",2.718281828));
}

void ExpNumber::UnInitSymbConstList()
{
	for (vector<ExpNumber*>::iterator it = SymbConstList.begin(); it != SymbConstList.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	SymbConstList.clear();
}

bool ExpNumber::IsNumberChar(char c)
{
	bool result = false;

	if( 0 != isdigit(c) )
	{
		result = true;
	}
	else if( '.' == c )
	{
		result = true;
	}

	return result;
}

bool ExpNumber::IsNumber(const string &str4Parse)
{
	if(0 == str4Parse.size()) return false;

	int num_dot = 0;
	for(string::size_type ix=0; ix!=str4Parse.size(); ix++)
	{
		if('-' == str4Parse[ix])
		{
			if(0 != ix) return false;
		}
		else if(0 != isdigit(str4Parse[ix]))
		{
			continue;
		}
        else if('.'==str4Parse[ix])
		{
			if( (0==ix) || (str4Parse.size()-1==ix) ) return false;
            if(0 == isdigit(str4Parse[ix-1])) return false;
			if(1 < ++num_dot) return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool ExpNumber::IsLegal()const
{
	return ( (IsNumber(strBody)) || (IsSymbConstNumber(strBody)) );
}

int ExpNumber::TransStrToNumber(double &receive, const string &str4Parse)
{
	double value=0;

	if(str4Parse.empty())
	{
		receive = 0;
		return -1;
	}

    bool withOppSgn = false;

	string str_temp = str4Parse;
	if('-' == str_temp[0])
	{
		str_temp = str_temp.substr(1, str_temp.size()-1);
		withOppSgn = true;
	}

    if(IsNumber(str_temp))
	{
		value = atof(str_temp.c_str());
	}

	if(withOppSgn)
	{
		value = -value;
	}

	receive = value;

	return 0;
}
int ExpNumber::GenerateValue()
{
	if(IsNumber(strBody))
	{
		return TransStrToNumber(this->value, this->strBody)==0 ? 0 : -1;
	}
    else
	{
		if(!IsInitSymbConstList()) InitSymbConstList();

		for(vector<ExpNumber*>::iterator iter=SymbConstList.begin(); iter!=SymbConstList.end(); iter++)
		{
			if(strBody == (*iter)->ReturnStrBody())
			{
				value = (*iter)->value;
				return 0;
			}
		}
	}

	return -3;
}

bool ExpNumber::SetValue(double v_value)
{
	// The symbol const number and numric number can not be set value.
	if (IsSymbConstNumber(this->ReturnStrBody()))
		return false;

	if (!ExpIdentifier::IsIdentifier(this->ReturnStrBody()))
		return false;

	this->value = v_value;
	this->hasValidValue = true;

	return true;
}

double ExpNumber::Value()const
{
	if (hasValidValue)
		return value;
	else
	{
		string errMsg = string("The number \"") + this->ReturnStrBody() + "\" has no value.";
		ErrorManager::GetInstance()->PushErrorInfo(errMsg);
		return 0;
	}
}

bool ExpNumber::IsSymbConstNumber(const string& str)
{
	if(!IsInitSymbConstList()) InitSymbConstList();

	if(ExpNumber::SymbConstList.empty()) return false;

	for(vector<ExpNumber*>::iterator iter=SymbConstList.begin(); iter!=SymbConstList.end(); iter++)
	{
		if((*iter)->ReturnStrBody() == str) return true;
	}
	return false;
}

int ExpBracket::upPriority = 10000;

ExpIdentifier::ExpIdentifier(const string &str):ExpressPart(str)
{
}

bool ExpIdentifier::IsLegal()
{
	return (IsIdentifier(strBody));
}

bool ExpIdentifier::IsIdentifierChar(const char &c)
{
	bool result = false;

	if( 0 != isalpha(c) )
	{
		result = true;
	}
	else if( 0 != isdigit(c) )
	{
		result = true;
	}

	return result;
}

bool ExpIdentifier::IsIdentifier(const string &str4Parse)
{
	if( 0 == str4Parse.size() ) return false;

	if( 0 == isalpha(str4Parse[0]) ) return false;  //Ê××Ö·û²»ÊÇ×ÖÄ¸£¬Ôò²»ÊÇ±êÊ¶·û

	for(string::size_type ix=0; ix!=str4Parse.size(); ix++)
	{
		if( false == IsIdentifierChar(str4Parse[ix]) ) return false;
	}
	return true;
}

bool ExpIdentifier::IsOperator()
{
	return ExpOperator::IsOperator(strBody);
}

bool ExpIdentifier::IsSymbConstNumber()
{
	return ExpNumber::IsSymbConstNumber(strBody);
}

} //namespace zhcosin
