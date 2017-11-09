#ifndef _USER_FUNCTOR_H_
#define _USER_FUNCTOR_H_

#include "OperatorFunction.h"
#include "Express.h"

namespace zhcosin
{

class user_functor : public operator_functor
{
public:
	user_functor(const string &name, const string &body, const vector<string> &args);
	~user_functor();
public:
	double operator()(vector<double> &parameters)const;
	string ToString()const;
private:
	string name;
	string str;
	Express e;
	vector<ExpNumber*> args;
};

} // namespace zhcosin

#endif // _USER_FUNCTOR_H_