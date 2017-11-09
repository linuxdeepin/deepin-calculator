
#ifndef _PARSER_H_
#define _PARSER_H_

#include "includes.h"
#include "ExpressPart.h"
#include "Singleton.h"
#include "IService.h"

namespace zhcosin
{
using namespace std;

////////////////////////////////////////////////////////////////
class WordParseProcessor : public Singleton<WordParseProcessor>
{
	DECLARE_SINGLETON(WordParseProcessor)
private:
	WordParseProcessor();
	~WordParseProcessor(){};
public:
	int Parse(const string &str4Parse,list<ExpressPart*> &container, const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions);
public:
	bool IsIdentifier(const string &str4Parse);
	bool IsNumber(const string &str4Parse);
	bool IsOperator(const string &str4Parse);
private:
	int PrepareParse(const string &str4Parse);
	int MainParse(const string &str4Parse,list<ExpressPart*> &container);
	int CompleteParse(list<ExpressPart*> &container,const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions);
private:
	bool CheckBracketMath(const string &str);
	int ProcessIdentifier(list<ExpressPart*> &container,const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions);
};

////////////////////////////////////////////////////////////////
class WordParseService : public IService,
			 public Singleton<WordParseService>
{
	DECLARE_SINGLETON(WordParseService)
private:
	WordParseService(){}
	~WordParseService(){}

public:
	virtual int StartService();
	virtual int StopService();

public:
	WordParseProcessor* GetWordParser();
};

///////////////////////////////////////////////////////////////////
class SyntaxParseProcessor : public Singleton<SyntaxParseProcessor>
{
	DECLARE_SINGLETON(SyntaxParseProcessor)
private:
	SyntaxParseProcessor(){}
	~SyntaxParseProcessor(){}
public:
	int Parse(list<ExpressPart*> &container);

private:
	int PrepareParse(list<ExpressPart*> &container);
	int ProcessFactorialLocation(list<ExpressPart*> &container);
	int ProcessSubtractAndOpposite(list<ExpressPart*> &container);
	int processPriorityOfOpt(list<ExpressPart*> &container);
	int ParseStep(list<ExpressPart*> &container,const list<ExpressPart*>::iterator it_beg,const list<ExpressPart*>::iterator it_end);
	bool CheckBracket(list<ExpressPart*> &container,list<ExpressPart*>::iterator it_beg,list<ExpressPart*>::iterator it_end);

private:
	list<ExpressPart*>::iterator previous_iterator(list<ExpressPart*>::iterator iter);
	list<ExpressPart*>::iterator next_iterator(list<ExpressPart*>::iterator iter);
	bool iterator_compare_lessthan(list<ExpressPart*>& container,list<ExpressPart*>::iterator iter1,list<ExpressPart*>::iterator iter2);
};

///////////////////////////////////////////////////////////////////
class SyntaxParseService : public IService,
			   public Singleton<SyntaxParseService>
{
	DECLARE_SINGLETON(SyntaxParseService)
private:
	SyntaxParseService(){}
	~SyntaxParseService(){}

public:
	int StartService();
	int StopService();

public:
	SyntaxParseProcessor *GetSyntaxParser();
};

////////////////////////////////////////////////////
class Computor : public Singleton<Computor>
{
	DECLARE_SINGLETON(Computor)
private:
	Computor(){}
	~Computor(){}
public:
	double Value(list<ExpressPart*> &container);

private:
	double ValueStep(list<ExpressPart*> &container, const list<ExpressPart*>::iterator ExpBeg, const list<ExpressPart*>::iterator ExpEnd);

private:
	list<ExpressPart*>::iterator previous_iterator(list<ExpressPart*>::iterator iter);
	list<ExpressPart*>::iterator next_iterator(list<ExpressPart*>::iterator iter);
};

////////////////////////////////////////////////////
class ComputeService : public IService,
		       public Singleton<ComputeService>
{
	DECLARE_SINGLETON(ComputeService)
private:
	ComputeService(){}
	~ComputeService(){}

public:
	int StartService(){return 0;}
	int StopService(){return 0;}

public:
	Computor *GetComputor(){return Computor::GetInstance();}
};

} // namespace zhcosin

#endif // _PARSER_H_
