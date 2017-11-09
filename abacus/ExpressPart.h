
#ifndef _ExpressPart_H_
#define _ExpressPart_H_

#include "includes.h"
#include "OperatorFunction.h"

namespace zhcosin
{

using std::string;
using std::vector;
using std::list;

class ExpressPart
{
public:
	ExpressPart(){strBody="";}
	virtual ~ExpressPart(){}
	ExpressPart(string strBody)
	{
        this->strBody=strBody;
	}

public:
	virtual bool IsLegal()const
	{
		return !strBody.empty();
	}

	string ReturnStrBody()const{return strBody;}

public:
	static int DestroyVectorOfExpressPartPtr(list<ExpressPart*> &container);

protected:
	string 			strBody;
};

//typedef double (*OptFunctor)(const vector<double>&);

class ExpOperator : public ExpressPart
{
public:
	ExpOperator();
	ExpOperator(const string &opt);
    ~ExpOperator() {}
	ExpOperator(const string &opt,const int nNumber,const int priority,operator_functor *functor);	

public:
	static bool IsInitOperatorList();
	static bool InitOperatorList();
	static bool UnInitOperatorList();
	static bool IsOperator(const string &str4Parse);
	static int ReturnAnyNumbers(){return AnyNumbers;}

public:
	bool IsLegal()const;
	bool IsCommonOperator()const;
	bool IsFunctionOperator()const;
	int Priority()const{return priority;}
	int NumOfNumbers()const{return nNumber;}
	int SetPriority(const int n){priority = n;return 0;}
	operator_functor* GetFunctor()const {return functor;}

private:
	int 	nNumber;			// The number of objects for this operator.
    int		priority;
	operator_functor				*functor;			// The function of complete compution.
	static vector<ExpOperator*>		OperatorList;		// List of built-in operator.
	static int		AnyNumbers;			// The pNumber for operator who has any numbers.
};

class ExpNumber : public ExpressPart
{
public:
	ExpNumber(){value = 0; hasValidValue = false;}
	ExpNumber(const double &f){value = f; hasValidValue = true;}
	ExpNumber(const string &str);
	ExpNumber(const string &str, double v_value):ExpressPart(str){value = v_value; hasValidValue = true;}
	~ExpNumber(){}

public:
	static bool IsNumber(const string &str4Parse);
	static void InitSymbConstList();
	static void UnInitSymbConstList();
	static bool IsInitSymbConstList();
	static bool IsSymbConstNumber(const string& str);

public:
	bool IsLegal()const;
	bool ValueIsValid()const{return hasValidValue;}
	bool SetValue(double v_value);
	double Value()const;

public:
	static int TransStrToNumber(double &receive, const string &str4Parse);

private:
	static bool IsNumberChar(char c);
	int GenerateValue();

private:
	bool	hasValidValue;
	double	value;
	static vector<ExpNumber*>	SymbConstList;		// List of constant numbers.
};

class ExpBracket : public ExpressPart
{
public:
	ExpBracket(){}
	ExpBracket(const string &str):ExpressPart(str){}
	~ExpBracket(){}
public:
	static bool IsBracket(const string &str4Parse){return ("("==str4Parse||")"==str4Parse);}

public:
	bool IsLegal()const{return IsBracket(strBody);}
	bool IsLeftBracket(){return "("==strBody;}
	bool IsRightBracket(){return ")"==strBody;}

public:
	static int upPriority;		// The base for upraise the priority of operator.
};

class ExpComma : public ExpressPart
{
public:
	ExpComma():ExpressPart(","){}
	~ExpComma(){}

public:
	bool IsLegal(){return ","==strBody;}
	bool IsComma(){return ","==strBody;}
};

class ExpIdentifier : public ExpressPart
{
public:
	ExpIdentifier(){}
	ExpIdentifier(const string &str);
	~ExpIdentifier(){}

public:
	static bool IsIdentifier(const string &str4Parse);

public:
	bool IsLegal();
	bool IsOperator();
	bool IsSymbConstNumber();

private:
	static bool IsIdentifierChar(const char &c);
};

} //namespace zhcosin

#endif //_ExpressPart_H_

