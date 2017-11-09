#include "Express.h"

#include "ExpressPart.h"
#include "Logger.h"
#include "parser.h"

namespace zhcosin
{

using namespace std;

Express::Express()
{
}

Express::Express(const Express &other)
{
	ExpressPart *newItem = NULL;
	ExpressPart *oldItem = NULL;
	for (list<ExpressPart*>::const_iterator it = other.container.begin(); it != other.container.end(); it++)
	{
		oldItem = *it;
		if (typeid(*oldItem) == typeid(ExpOperator))
		{
			ExpOperator *pOperator = dynamic_cast<ExpOperator*>(oldItem);
			newItem = new ExpOperator(*pOperator);
        }
		else if (typeid(*oldItem) == typeid(ExpNumber))
		{
			ExpNumber *pOperator = dynamic_cast<ExpNumber*>(oldItem);
			newItem = new ExpNumber(*pOperator);
		}
		else if (typeid(*oldItem) == typeid(ExpBracket))
		{
			ExpBracket *pOperator = dynamic_cast<ExpBracket*>(oldItem);
			newItem = new ExpBracket(*pOperator);
		}
		else if (typeid(*oldItem) == typeid(ExpComma))
		{
			ExpComma *pOperator = dynamic_cast<ExpComma*>(oldItem);
			newItem = new ExpComma(*pOperator);
		}
		else if (typeid(*oldItem) == typeid(ExpIdentifier))
		{
			ExpIdentifier *pOperator = dynamic_cast<ExpIdentifier*>(oldItem);
			newItem = new ExpIdentifier(*pOperator);
		}
		else
		{
			this->Destroy();
			break;
		}

		this->container.push_back(newItem);
	}
}

Express::~Express()
{
	Destroy();
}

bool Express::IsLegal()const
{
	return !container.empty();
}

int Express::SyntaxParse()
{
	return SyntaxParseService::GetInstance()->GetSyntaxParser()->Parse(container);
}

double Express::Value()
{
	return ComputeService::GetInstance()->GetComputor()->Value(container);
}

string Express::ReturnExpStr()const
{
	string str;
	for(list<ExpressPart*>::const_iterator it=container.begin(); it!=container.end(); it++)
	{
		str += (*it)->ReturnStrBody();
		str += " ";
	}
	return str;
}

int Express::WordParse(const string &str, const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions)
{
	return WordParseService::GetInstance()->GetWordParser()->Parse(str, container, userVariables, userFunctions);
}

int Express::Destroy()
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

void Express::ShowContainer()const
{
	cout<<endl<<"===============show================"<<endl;
	if(container.empty())
	{
		cout<<"container is empty."<<endl;
		return;
	}

	for(list<ExpressPart*>::const_iterator iter=container.begin(); iter!=container.end(); iter++)
	{
		if(typeid(*(*iter))==typeid(ExpOperator))
		{
			cout<<"ExpOperator ";
			ExpOperator *opt=dynamic_cast<ExpOperator*>(*iter);
			cout<<((opt->IsCommonOperator())?"Common ":((opt->IsFunctionOperator())?"Function ":"UnknownOpt "))<<opt->Priority()<<endl;
		}
		else if(typeid(*(*iter))==typeid(ExpNumber))
		{
			cout<<"ExpNumber ";
			ExpNumber *num=dynamic_cast<ExpNumber*>(*iter);
			cout<<"value="<<num->Value()<<endl;
		}
		else if(typeid(*(*iter))==typeid(ExpBracket)) cout<<"ExpBracket "<<endl;
		else if(typeid(*(*iter))==typeid(ExpComma)) cout<<"ExpComma "<<endl;
		else if(typeid(*(*iter))==typeid(ExpIdentifier)) cout<<"ExpIdentifier "<<endl;
		else cout<<"unknow ";
	}
	cout<<endl;
}

bool Express::ReplaceFormatArgsWithRealArgs( const vector<ExpNumber*> &formatArgs, const vector<double> &realArgs )
{
	vector<ExpNumber*>::const_iterator it_fomat_arg = formatArgs.begin();
	vector<double>::const_iterator it_real_arg = realArgs.begin();
	list<ExpressPart*>::iterator it;

	while (it_fomat_arg != formatArgs.end() && it_real_arg != realArgs.end())
	{
		// TODO: replace fomat arguments with real arguments.
		for (it = container.begin(); it != container.end(); it++)
		{
			ExpressPart *curtItem = *it;
			if (typeid(*curtItem) == typeid(ExpNumber)
				&& curtItem->ReturnStrBody() == (*it_fomat_arg)->ReturnStrBody())
			{
				delete curtItem;
				curtItem = new ExpNumber(*it_real_arg);
				*it = curtItem;
			}
		}
		it_fomat_arg++;
		it_real_arg++;
	}

	return true;
}
} // namespace zhcosin
