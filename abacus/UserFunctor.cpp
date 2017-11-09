#include "UserFunctor.h"
#include "Session.h"

namespace zhcosin
{

user_functor::user_functor( const string &name, const string &body, const vector<string> &args )
{
	this->name = name;
	this->str = body;

	// create format arguments.
	for (vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		this->args.push_back(new ExpNumber(*it));
	}

    this->e.WordParse(this->str, this->args, Session::GetInstance()->GetUserFunctions());
	this->e.SyntaxParse();
}

user_functor::~user_functor()
{
	e.Destroy();

	for (vector<ExpNumber*>::iterator it = args.begin(); it != args.end(); it++)
	{
		delete *it;
		*it = NULL;
	}
	args.clear();
}

double user_functor::operator()( vector<double> &parameters )const
{
	if (parameters.size() != args.size())
	{
		// TODO: The number of format arguments and real arguments is not match.
	}

	Express e_copy(this->e);
	if (!e_copy.IsLegal())
	{
		// TODO: failed to copy Express.
	}

	// replace fomat arguments with real arguments.
	if (!e_copy.ReplaceFormatArgsWithRealArgs(args, parameters))
	{
		// TODO: failed to replace format argcuments with real arguments.
	}

	return e_copy.Value();
}

string user_functor::ToString()const
{
	string result;
	result += this->name;

	result += "(";
	for (vector<ExpNumber*>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		if (it != args.begin())
			result += ",";
		result += (*it)->ReturnStrBody();
	}
	result += ")";
	result += " = ";
	result += this->str;

	return result;
}

} // namespace zhcosin

