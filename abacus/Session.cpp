#include "Session.h"
#include "UserFunctor.h"
#include <algorithm>

namespace zhcosin
{

bool Session::AddUserVariable( ExpNumber *theVariable )
{
	ExpNumber *theMyVariable = new ExpNumber(*theVariable);
	userVariables.push_back(theMyVariable);
	return true;
}

bool Session::RemoveUserVariable( const string &nameOfVariable )
{
	vector<ExpNumber*>::iterator it;
	for (it = userVariables.begin(); it != userVariables.end(); it++)
	{
		if (nameOfVariable == (*it)->ReturnStrBody())
		{
			delete *it;
			*it = NULL;
			userVariables.erase(it);
			return true;
		}
	}
	return false;
}

bool Session::RemoveAllUserVariables()
{
	vector<ExpNumber*>::iterator it;
	for (it = userVariables.begin(); it != userVariables.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	userVariables.clear();
	return true;
}

bool Session::SetValueOfVariable(const string &name, double value)
{
	for (vector<ExpNumber*>::iterator it = userVariables.begin(); it != userVariables.end(); it++)
	{
		if ((*it)->ReturnStrBody() == name)
		{
			return (*it)->SetValue(value);
		}
	}

	return false;
}

bool Session::AddUserFunction( ExpOperator *theOperator )
{
	ExpOperator *theMyOperator = new ExpOperator(*theOperator);
	userFuns.push_back(theMyOperator);
	return true;
}

string Session::GetFunString(const string &name)const
{
	operator_functor *theOptFunctor = NULL;
	user_functor *theUserFunctor = NULL;

	if (name.empty())
		return string("");

	for (vector<ExpOperator*>::const_iterator it = userFuns.begin(); it != userFuns.end(); it++)
	{
		if ((*it)->ReturnStrBody() == name)
		{
			theOptFunctor = (*it)->GetFunctor();
			if (typeid(*theOptFunctor) == typeid(user_functor))
			{
				theUserFunctor = dynamic_cast<user_functor*>(theOptFunctor);
					return theUserFunctor->ToString();
			}
			else
			{
				return string("");
			}
		}
	}

	return string("");
}

bool Session::RemoveUserFunction( const string &nameOfFunction )
{
	vector<ExpOperator*>::iterator it;
	for (it = userFuns.begin(); it != userFuns.end(); it++)
	{
		if (nameOfFunction == (*it)->ReturnStrBody())
		{
			delete *it;
			*it = NULL;
			userFuns.erase(it);
			return true;
		}
	}
	return false;
}

bool Session::RemoveAllUserFunctions()
{
	vector<ExpOperator*>::iterator it;
	for (it = userFuns.begin(); it != userFuns.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	userFuns.clear();
	return true;
}

bool Session::LoadPreDefindVariables()
{
	// TODO:
	return true;
}
bool Session::LoadPreDefindFunctions()
{
	// TODO:
	return true;
}

const vector<ExpNumber*>&  Session::GetUserVariables() const
{
	return this->userVariables;
}

const vector<ExpOperator*>&  Session::GetUserFunctions() const
{
	return this->userFuns;
}

Session::Session()
{

}

Session::~Session()
{
	this->RemoveAllUserVariables();
	this->RemoveAllUserFunctions();
}

} // namespace zhcosin
