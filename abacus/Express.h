
#ifndef _EXPRESS_H_
#define _EXPRESS_H_
#include "includes.h"
#include "ExpressPart.h"

namespace zhcosin
{

using std::string;
using std::list;

class Express
{
public:
	Express();
	Express(const Express &e);
	~Express();
public:
	int		WordParse(const string &str, const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions);
	int		SyntaxParse();
	double 	Value();
	string	ReturnExpStr()const;
	bool	IsLegal()const;
	int		Destroy();
	void	ShowContainer()const;
	bool	ReplaceFormatArgsWithRealArgs(const vector<ExpNumber*> &formatArgs, const vector<double> &realArgs);

private:
    list<ExpressPart*>	container;
};

}

#endif

