
#ifndef _OPERATORFUNCTION_H_
#define _OPERATORFUNCTION_H_

#include "includes.h"
#include <cmath>
#include <functional>
#include "ErrorManager.h"

namespace zhcosin
{

using namespace std;

class operator_functor
{
public:
	operator_functor(){}
	virtual ~operator_functor(){}
	virtual double operator()(vector<double> &parameters)const = 0;
};

class plus_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		plus<double> plus_std_functor;
		return plus_std_functor(parameters[0], parameters[1]);
	}
};

class minus_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		minus<double> minus_std_functor;
		return minus_std_functor(parameters[0], parameters[1]);
	}
};

class negate_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		negate<double> negate_std_functor;
		return negate_std_functor(parameters[0]);
	}
};

class multiplies_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		multiplies<double> multiplies_std_functor;
		return multiplies_std_functor(parameters[0], parameters[1]);
	}
};

class divides_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		divides<double> divides_std_functor;
		if (abs(parameters[1]) < 10e-9)
		{
			string errMsg = "A error of \"divisor is zero\" has occur.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return 0;
		}
		return divides_std_functor(parameters[0], parameters[1]);
	}
};

class modulus_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		modulus<int> modulus_std_functor;
		if ((int)parameters[1] <= 0)
		{
			string errMsg = "The second parameter of modulus must be positive.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return 0;
		}
		return modulus_std_functor((int)parameters[0], (int)parameters[1]);
	}
};

class abs_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return abs(parameters[0]);
	}
};

class pow_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return pow(parameters[0], parameters[1]);
	}
};

class exp_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return exp(parameters[0]);
	}
};

class log_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		if (parameters[0] <= 0 || parameters[1] <= 0)
		{
			string errMsg = "The parameters of log() both must be positive.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return 0;
		}
		return log(parameters[0]) / log(parameters[1]);
	}
};

class ln_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		if (parameters[0] <= 0)
		{
			string errMsg = "The parameter of ln() must be positive.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return 0;
		}
		return log(parameters[0]);
	}
};

class sqrt_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		if (parameters[0] < 0)
		{
			string errMsg = "The parameter of sqrt() must be non-negative.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return 0;
		}

		return sqrt(parameters[0]);
	}
};

class sin_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return sin(parameters[0]);
	}
};

class cos_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return cos(parameters[0]);
	}
};

class tan_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return tan(parameters[0]);
	}
};

class arcsin_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return asin(parameters[0]);
	}
};

class arccos_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return acos(parameters[0]);
	}
};

class arctan_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return atan(parameters[0]);
	}
};

class factorial_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		int n = (int)parameters[0];

		if (n < 0)
			return -1;

		if (n == 0)
			return 1;

		int i, result = 1;
        for (i = 1; i <= n; i++)
			result *= i;

		return result;
	}
};

class double_factorial_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		int n = (int)parameters[0];

		if (n < 0)
			return -1;

		if (n == 0)
			return 0;

		int i, result = 1;
        for (i = (n % 2 == 0 ? 2 : 1); i <= n; i += 2)
			result *= i;

		return result;
	}
};

class cb_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		int m = (int)parameters[0];
		int n = (int)parameters[1];

		if (n <= 0 || m <= 0 || m > n)
			return 0;

		int numerator = 1;
		int denominator  = 1;
		for (int i = 0; i < m; i++)
		{
			numerator *= (n-i);
			denominator *= (m-i);
		}

		return (double)numerator / (double)denominator;
	}
};

class max_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		double curtMax = parameters[0];
		for (vector<double>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
		{
			if (*it > curtMax)
				curtMax = *it;
		}

		return curtMax;
	}
};

class min_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		double curtMax = parameters[0];
		for (vector<double>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
		{
			if (*it < curtMax)
				curtMax = *it;
		}

		return curtMax;
	}
};

class ceil_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return ceil(parameters[0]);
	}
};

class floor_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return floor(parameters[0]);
	}
};

class sinh_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return sinh(parameters[0]);
	}
};

class cosh_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return cosh(parameters[0]);
	}
};

class tanh_functor : public operator_functor
{
public:
	double operator()(vector<double> &parameters)const
	{
		return tanh(parameters[0]);
	}
};


}


#endif

