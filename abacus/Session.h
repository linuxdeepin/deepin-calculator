#ifndef _SESSION_H_
#define _SESSION_H_

#include "includes.h"
#include "ExpressPart.h"
#include "Singleton.h"

namespace zhcosin
{

class Session : public Singleton<Session>
{
	DECLARE_SINGLETON(Session)
private:
	Session();
public:
	~Session();
public:
	bool AddUserVariable(ExpNumber *theVariable);
	bool RemoveUserVariable(const string &nameOfVariable);
	bool RemoveAllUserVariables();
	bool SetValueOfVariable(const string &name, double value);
	bool AddUserFunction(ExpOperator *theOperator);
	string GetFunString(const string &name)const;
	bool RemoveUserFunction(const string &nameOfFunction);
	bool RemoveAllUserFunctions();
	bool LoadPreDefindVariables();
	bool LoadPreDefindFunctions();
	const vector<ExpNumber*> &		GetUserVariables()const;
	const vector<ExpOperator*> &	GetUserFunctions()const;
private:
	vector<ExpNumber*>		userVariables;
	vector<ExpOperator*>	userFuns;
};

} // namespace zhcosin

#endif // _SESSION_H_