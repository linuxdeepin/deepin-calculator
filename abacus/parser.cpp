
#include <iostream>
#include "includes.h"
#include "parser.h"
#include "ExpressPart.h"
#include "OperatorFunction.h"
#include "ErrorManager.h"

namespace zhcosin
{
using namespace std;

extern plus_functor plusor;
extern minus_functor minusor;
extern negate_functor negator;
extern multiplies_functor multiplor;
extern divides_functor dividor;

////////////////////////////////////////////////////////////////
WordParseProcessor::WordParseProcessor()
{
	//ExpOperator::InitOperatorList();
	//ExpNumber::InitSymbConstList();
}

bool WordParseProcessor::CheckBracketMath(const string &str)
{
	if(str.empty()) return false;

	int n=0;
	for(string::size_type ix=0; ix!=str.size(); ix++)
	{
		if('('==str[ix]) n++;
		else if(')'==str[ix])
		{
			if(0>--n) return false;
		}
	}

	return 0==n;
}

int WordParseProcessor::MainParse(const string &str4Parse,list<ExpressPart*> &container)
{
	if(str4Parse.empty())
	{
		return -1;
	}

	if(!ExpOperator::IsInitOperatorList()) ExpOperator::InitOperatorList();
	if(!ExpNumber::IsInitSymbConstList()) ExpNumber::InitSymbConstList();

	string::size_type ix=0;
	while(ix<str4Parse.size())
	{
		if(' ' == str4Parse[ix])
		{// Ignore the space character.
			ix++;
			continue;
		}

		if(0!=isdigit(str4Parse[ix]))
		{
            string::size_type jx;
            for(jx=ix+1; jx<=str4Parse.size(); jx++)
			{
				if(str4Parse.size()!=jx)
				{
					if(str4Parse.size()==1+jx)
					{
						if(
							(!IsNumber(str4Parse.substr(ix,jx-ix+1)))
                            &&(IsNumber(str4Parse.substr(ix,jx-ix)))
						  )
						{
							break;
						}
					}

					if(
						(!IsNumber(str4Parse.substr(ix,jx-ix+1)))
                        &&(!IsNumber(str4Parse.substr(ix,jx-ix)))
					  )
					{
						jx--;
						break;
					}
				}
			}
			container.push_back(new ExpNumber(str4Parse.substr(ix,jx-ix)));
			ix=jx;
		}
		else if(0!=isalpha(str4Parse[ix]))	
		{
			string::size_type jx;
			for(jx=ix+1; jx<=str4Parse.size(); jx++)
			{
				if(!IsIdentifier(str4Parse.substr(ix,jx-ix)))
				{
					jx--;
					break;
				}
			}
			container.push_back(new ExpIdentifier(str4Parse.substr(ix,jx-ix)));
			ix=jx;
		}
		else
		{
			if(('('==str4Parse[ix])||(')'==str4Parse[ix]))
			{
				container.push_back(new ExpBracket(str4Parse.substr(ix,1)));
				ix++;
			}
			else if(','==str4Parse[ix])
			{
				container.push_back(new ExpComma());
				ix++;
			}
			else
			{
				bool isopt=false;
				for(string::size_type jx=str4Parse.size(); jx>ix; jx--)
				{
					if(IsOperator(str4Parse.substr(ix,jx-ix)))
					{
						container.push_back(new ExpOperator(str4Parse.substr(ix,jx-ix)));
						ix=jx;
						isopt=true;
						break;
					}
				}

				if (!isopt)
				{
					string errMsg = string("The string \"") + str4Parse[ix] + "\" is not a right component of a express.";
					ErrorManager::GetInstance()->PushErrorInfo(errMsg);
					return -3;
				}
			}
		}
	} //end while
	return 0;
}

bool WordParseProcessor::IsOperator(const string &str4Parse)
{
	return ExpOperator::IsOperator(str4Parse);
}

bool WordParseProcessor::IsNumber(const string &str4Parse)
{
	return ExpNumber::IsNumber(str4Parse);
}

bool WordParseProcessor::IsIdentifier(const string &str4Parse)
{
	return ExpIdentifier::IsIdentifier(str4Parse);
}

int WordParseProcessor::ProcessIdentifier(list<ExpressPart*> &container,const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions)
{
	if(container.empty()) return -1;

	for(list<ExpressPart*>::iterator iter=container.begin(); iter!=container.end(); iter++)
	{
		if(typeid(ExpIdentifier)==typeid(*(*iter)))
		{
			if(IsOperator((*iter)->ReturnStrBody()))
			{
				ExpOperator *opt = new ExpOperator((*iter)->ReturnStrBody());
				delete *iter;
				*iter = opt;
			}
			else if(ExpNumber::IsSymbConstNumber((*iter)->ReturnStrBody()))
			{
				ExpNumber *num = new ExpNumber((*iter)->ReturnStrBody());
				delete *iter;
				*iter = num;
			}
			else
			{
				bool isUserVariable = false;
				vector<ExpNumber*>::const_iterator itUserVariable;
				for(itUserVariable = userVariables.begin(); itUserVariable != userVariables.end(); itUserVariable++)
				{
					if ((*itUserVariable)->ReturnStrBody() == (*iter)->ReturnStrBody())
					{
						isUserVariable = true;
						ExpNumber *num = new ExpNumber(*(*itUserVariable));
						delete *iter;
						*iter = num;
						break;
					}
				}
				if (isUserVariable)
					continue;

				bool isUserFun = false;
				vector<ExpOperator*>::const_iterator itUserFun;
				for (itUserFun = userFunctions.begin(); itUserFun != userFunctions.end(); itUserFun++)
				{
					if ((*itUserFun)->ReturnStrBody() == (*iter)->ReturnStrBody())
					{
						isUserFun = true;
						ExpOperator *opt = new ExpOperator(*(*itUserFun));
						delete *iter;
						*iter = opt;
						break;
					}

				}
				if (isUserFun)
					continue;

				string errMsg = "Undefined identifier: \"" + (*iter)->ReturnStrBody() + "\".";
				ErrorManager::GetInstance()->PushErrorInfo(errMsg);
				return -2;
			}
		}
	}

	return 0;
}

int WordParseProcessor::PrepareParse(const string &str4Parse)
{
	bool isRightBracket = CheckBracketMath(str4Parse);
	if(!isRightBracket)
	{
		string errMsg = "The brackets in the express is not match.";
		ErrorManager::GetInstance()->PushErrorInfo(errMsg);
		return -1;
	}

	return 0;
}

int WordParseProcessor::CompleteParse(list<ExpressPart*> &container,const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions)
{
	return ProcessIdentifier(container, userVariables, userFunctions);
}

int WordParseProcessor::Parse(const string &str4Parse,list<ExpressPart*> &container,const vector<ExpNumber*> &userVariables, const vector<ExpOperator*> &userFunctions)
{
	int status_code=0;
	status_code = PrepareParse(str4Parse);
	if(0 != status_code) return -1;

	status_code = MainParse(str4Parse,container);
	if(0 != status_code) return -2;

	status_code = CompleteParse(container,userVariables, userFunctions);
	if(0 != status_code) return -3;

	return 0;
}

////////////////////////////////////////////////////////////////
int WordParseService::StartService()
{
	IService::StartService();

	ExpOperator::InitOperatorList();
	ExpNumber::InitSymbConstList();

	WordParseProcessor::GetInstance();

	return 0;
}

int WordParseService::StopService()
{
	ExpOperator::UnInitOperatorList();
	ExpNumber::UnInitSymbConstList();

	WordParseProcessor::DestroyInstance();

	return 0;
}

WordParseProcessor* WordParseService::GetWordParser()
{
	return WordParseProcessor::GetInstance();
}

///////////////////////////////////////////////////////////////
int SyntaxParseProcessor::PrepareParse(list<ExpressPart*> &container)
{
	int result=0;

	result = ProcessSubtractAndOpposite(container);
	if(result!=0) return -1;

	result = processPriorityOfOpt(container);
	if(result!=0) return -2;

	return 0;
}

int SyntaxParseProcessor::ProcessSubtractAndOpposite(list<ExpressPart*> &container)
{
	if(container.empty()) return 0;

	for(list<ExpressPart*>::iterator iter=container.begin(); iter!=container.end(); iter++)
	{
		if( (typeid(*(*iter))==typeid(ExpOperator)) && ((*iter)->ReturnStrBody()=="-") )
		{
			ExpOperator *opt=dynamic_cast<ExpOperator*>(*iter);
            if((container.begin()==iter)
				|| ((*(previous_iterator(iter)))->ReturnStrBody()=="(")
				|| ((*(previous_iterator(iter)))->ReturnStrBody()==","))
			{
				if(opt->Priority()!=2)
				{
					delete *iter;
					opt = NULL;
					*iter = NULL;
					*iter = new ExpOperator("-",1,2,(operator_functor*)&negator);
				}
			}
			else if((container.begin()!=iter) && (((*(previous_iterator(iter)))->ReturnStrBody()==")") || (typeid(*(*(previous_iterator(iter))))==typeid(ExpNumber))))
			{
				if(opt->Priority()!=1)
				{
					delete *iter;
					opt = NULL;
					*iter = NULL;
					*iter = new ExpOperator("-",2,1,(operator_functor*)&minusor);
				}
			}
			// else ?
		}
	}
	return 0;
}

int SyntaxParseProcessor::processPriorityOfOpt(list<ExpressPart*> &container)
{
	if(container.empty()) return 0;

	int level=0;
	for(list<ExpressPart*>::iterator iter=container.begin(); iter!=container.end(); iter++)
	{
		if(typeid(*(*iter))==typeid(ExpOperator))
		{
			ExpOperator *opt=dynamic_cast<ExpOperator*>(*iter);
			opt->SetPriority(opt->Priority()+level*ExpBracket::upPriority);
		}
		else if(typeid(*(*iter))==typeid(ExpBracket))
		{
			ExpBracket *bkt=dynamic_cast<ExpBracket*>(*iter);
			if(bkt->IsLeftBracket())
			{
				level++;
			}
			else if(bkt->IsRightBracket())
			{
				level--;
			}
			else
			{
				return -1;
			}
		}
	}

	return 0;
}

int SyntaxParseProcessor::Parse(list<ExpressPart*> &container)
{
	int status_code=0;
	status_code = PrepareParse(container);
	if(0 != status_code) return -1;

    status_code = ParseStep(container,container.begin(),container.end());
	if(0 != status_code) return -2;

	return 0;
}

int SyntaxParseProcessor::ParseStep(list<ExpressPart*> &container,const list<ExpressPart*>::iterator it_beg,const list<ExpressPart*>::iterator it_end)
{
	list<ExpressPart*>::iterator express_begin = it_beg;
	list<ExpressPart*>::iterator express_end = it_end;
	string errMsg;

	// Get the express string.
	string expStr;
	for(list<ExpressPart*>::const_iterator it_curt = it_beg; it_curt != it_end; it_curt++)
	{
		expStr += (*it_curt)->ReturnStrBody();
	}

	if(!iterator_compare_lessthan(container,express_begin,express_end))
	{
		errMsg = "Unknown error has be occur.";
		ErrorManager::GetInstance()->PushErrorInfo(errMsg);
		return -1;
	}

	if(express_end==next_iterator(express_begin))
	{
		if(typeid(*(*express_begin))==typeid(ExpNumber))
		{
			return 0;
		}
		else
		{
			errMsg = "The component \" " + (*express_begin)->ReturnStrBody() + "\" in \"" + expStr + "\" can not be computed.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return -2;
		}
	}

    if((((*(express_begin))->ReturnStrBody()=="(")&&((*(previous_iterator(express_end)))->ReturnStrBody()==")")&&CheckBracket(container,next_iterator(express_begin),previous_iterator(express_end))))
	{
		express_begin++;
		express_end--;
	}
	else
	{
		container.insert(express_begin,new ExpBracket("("));
		container.insert(express_end,new ExpBracket(")"));
		express_end--;
	}

	int lowest_priority=999999;
	ExpOperator *ptr_prime_opt=NULL,*ptr_temp_opt=NULL;
    list<ExpressPart*>::iterator it=express_end;
	for(list<ExpressPart*>::iterator iter=express_begin; iter!=express_end; iter++)
	{
		if(typeid(*(*iter))==typeid(ExpOperator))
		{
			ptr_temp_opt=dynamic_cast<ExpOperator*>(*iter);
			if(ptr_temp_opt->Priority()<=lowest_priority)
			{
				ptr_prime_opt=ptr_temp_opt;
				it=iter;
				lowest_priority=ptr_temp_opt->Priority();
			}
		}
	}
	if((NULL==ptr_prime_opt)||(express_end==it))
	{
		errMsg = "There is no primary operator in express \"" + expStr + "\".";
		ErrorManager::GetInstance()->PushErrorInfo(errMsg);
        return -3;
	}

	if(ptr_prime_opt->IsCommonOperator())
	{
		if(1==ptr_prime_opt->NumOfNumbers())
		{
			if(express_begin!=it)
			{
				if(((*it)->ReturnStrBody()=="!" || (*it)->ReturnStrBody()=="!!") && express_end==next_iterator(it))
				{
					container.insert(express_begin, *it);
					*it = NULL;
					container.erase(it);
					return ParseStep(container, express_begin, express_end);
				}
				else
				{
					errMsg = "The operator \"" + ptr_prime_opt->ReturnStrBody() + "\" has not be located rightly.";
					ErrorManager::GetInstance()->PushErrorInfo(errMsg);
                    return -4;
				}
			}
			list<ExpressPart*>::iterator it_begin_of_first_num = next_iterator(it);
			return ParseStep(container,it_begin_of_first_num,express_end);
		}
		else if(2==ptr_prime_opt->NumOfNumbers())
		{
			if(express_begin==it||previous_iterator(express_end)==it)
			{
				errMsg = "The operator \"" + (*it)->ReturnStrBody() + "\" has not be located rightly.";
				ErrorManager::GetInstance()->PushErrorInfo(errMsg);
                return -5;
			}

			list<ExpressPart*>::iterator it_end_of_first_num = next_iterator(it);
			list<ExpressPart*>::iterator it_begin_of_second_num = next_iterator(it);

			container.insert(express_begin,*it);
			*it = NULL;
			container.erase(it);

			int status_code=ParseStep(container,express_begin,it_end_of_first_num);
			if(0 != status_code)
			{
				return status_code;
			}

			status_code=ParseStep(container,it_begin_of_second_num,express_end);
			if(0 != status_code)
			{
				return status_code;
			}

			return 0;
		}
	}
	else if(ptr_prime_opt->IsFunctionOperator())
	{
		if(it!=express_begin)
		{
			errMsg = "The operator \"" + (*it)->ReturnStrBody() + "\" has not be located rightly.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return -10;
		}

        express_begin++;
		if(!(((*(express_begin))->ReturnStrBody()=="(")&&((*(previous_iterator(express_end)))->ReturnStrBody()==")")&&CheckBracket(container,next_iterator(express_begin),previous_iterator(express_end))))	
		{
			errMsg = "The function \"" + (*it)->ReturnStrBody() + "\" has not take brackets rightly.";
			ErrorManager::GetInstance()->PushErrorInfo(errMsg);
			return -11;
		}

		list<ExpressPart*>::iterator it_params_left_bracket = express_begin;
        list<ExpressPart*>::iterator it_params_right_bracket = express_end;

		list<ExpressPart*>::iterator it_params_begin = next_iterator(express_begin);
		list<ExpressPart*>::iterator it_params_end = previous_iterator(express_end);

        int n_numbers = ptr_prime_opt->NumOfNumbers();
        int n_params=0;
        int level=0;
		list<ExpressPart*>::iterator param_begin,param_end;
        param_begin = it_params_begin;
		param_end = it_params_end;
		bool isEndOfParam = false;
		for(list<ExpressPart*>::iterator it_loop=it_params_begin; it_loop!=next_iterator(it_params_end); it_loop++)
		{
			if(it_loop == it_params_end)
			{
				isEndOfParam = true;
				param_end = it_params_end;
			}
			else if("("==(*it_loop)->ReturnStrBody()) level++;
			else if(")"==(*it_loop)->ReturnStrBody()) level--;

			if(0!=level) continue;

			if (typeid(*(*it_loop))==typeid(ExpComma))
			{
				isEndOfParam = true;
				param_end = it_loop;
			}

			if(isEndOfParam)
			{
				n_params++;
				if(n_params > n_numbers)
				{
					errMsg = "The function \"" + (*it)->ReturnStrBody() + "\" has take too many parameters.";
					ErrorManager::GetInstance()->PushErrorInfo(errMsg);
					return -30;
				}

				if(0 != ParseStep(container,param_begin,param_end))
				{
					return -20;
				}
				param_begin = next_iterator(it_loop);
				isEndOfParam = false;
			}
		}

        if(n_numbers != ExpOperator::ReturnAnyNumbers())
		{
			if(n_params < n_numbers)
			{
				errMsg = "The function \"" + (*it)->ReturnStrBody() + "\" has take too few parameters.";
				ErrorManager::GetInstance()->PushErrorInfo(errMsg);
				return -30;
			}
		}

		level=0;
		it_params_begin = next_iterator(it_params_left_bracket);
		it_params_end = previous_iterator(it_params_right_bracket);

		list<ExpressPart*>::iterator it_comma;
		for(list<ExpressPart*>::iterator it_loop=it_params_begin; it_loop!=it_params_end; it_loop++)
		{
			if(typeid(*(*it_loop))==typeid(ExpBracket))
			{
				if("("==(*it_loop)->ReturnStrBody()) level++;
				else if(")"==(*it_loop)->ReturnStrBody()) level--;
				continue;
			}

			if(typeid(*(*it_loop))==typeid(ExpComma))
			{
				if(0==level)
				{
					it_comma=it_loop;
                    it_loop++;
					delete *it_comma;
					*it_comma=NULL;
					container.erase(it_comma);
					it_loop--;
				}
			}
		}

		list<ExpressPart*>::iterator it_params_right_bracket_body = previous_iterator(it_params_right_bracket);
		delete *it_params_left_bracket;
		delete *it_params_right_bracket_body;
		*it_params_left_bracket = NULL;
		*it_params_right_bracket_body = NULL;
		container.erase(it_params_left_bracket);
		container.erase(it_params_right_bracket_body);
	}
	else
	{
		errMsg = "Undefined operator \"" + (*it)->ReturnStrBody() + "\" in express \"" + expStr + "\".";
		ErrorManager::GetInstance()->PushErrorInfo(errMsg);
		return -100;
	}

	return 0;
}

bool SyntaxParseProcessor::CheckBracket(list<ExpressPart*> &container,list<ExpressPart*>::iterator it_beg,list<ExpressPart*>::iterator it_end)
{
	if(!iterator_compare_lessthan(container,it_beg,it_end)) return false;

	int level=0;
	for(list<ExpressPart*>::iterator iter=it_beg; iter!=it_end; iter++)
	{
		if((*iter)->ReturnStrBody()=="(")
		{
			level++;
		}
		else if((*iter)->ReturnStrBody()==")")
		{
			if(0>--level) return false;
		}

	}
	return 0==level;
}

list<ExpressPart*>::iterator SyntaxParseProcessor::previous_iterator(list<ExpressPart*>::iterator iter)
{
	list<ExpressPart*>::iterator temp_it=iter;
	return --temp_it;
}

list<ExpressPart*>::iterator SyntaxParseProcessor::next_iterator(list<ExpressPart*>::iterator iter)
{
	list<ExpressPart*>::iterator temp_it=iter;
	return ++temp_it;
}

bool SyntaxParseProcessor::iterator_compare_lessthan(list<ExpressPart*>& container,list<ExpressPart*>::iterator iter1,list<ExpressPart*>::iterator iter2)
{
	if(container.end()==iter1) return false;

	if(container.end()==iter2)
	{
		if(container.end()!=iter1) return true;
		else return false;
	}

	for(list<ExpressPart*>::iterator temp_it=next_iterator(iter1); temp_it!=container.end(); temp_it++)
	{
		if(temp_it == iter2) return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////
int SyntaxParseService::StartService()
{
	SyntaxParseProcessor::GetInstance();
	return 0;
}

int SyntaxParseService::StopService()
{
	SyntaxParseProcessor::DestroyInstance();
	return 0;
}

SyntaxParseProcessor* SyntaxParseService::GetSyntaxParser()
{
	return SyntaxParseProcessor::GetInstance();
}

double Computor::Value(list<ExpressPart*> &container)
{
	return ValueStep(container, container.begin(), container.end());
}

double Computor::ValueStep(list<ExpressPart*> &container, const list<ExpressPart*>::iterator ExpBeg, const list<ExpressPart*>::iterator ExpEnd)
{
	string errMsg;

	if(ExpEnd == next_iterator(ExpBeg))
	{
		ExpNumber *ptr_num = dynamic_cast<ExpNumber*>(*ExpBeg);
		return ptr_num->Value();
	}

	list<ExpressPart*>::iterator it_opt = next_iterator(ExpBeg);

	int level = 0;
	list<ExpressPart*>::iterator subExpBeg, subExpEnd;
	subExpBeg = ExpBeg;
	subExpEnd = ExpEnd;
	bool isEndSubExp = false;
	for(list<ExpressPart*>::iterator it_loop=next_iterator(it_opt); it_loop!=previous_iterator(ExpEnd); it_loop++)
	{
		if((*it_loop)->ReturnStrBody()=="(")
		{
			if(0==level)
			{
				subExpBeg = it_loop;
			}
			level++;
		}
		else if((*it_loop)->ReturnStrBody()==")")
		{
			level--;
			if(0==level)
			{
				subExpEnd = next_iterator(it_loop);
				it_loop = previous_iterator(subExpBeg);
				isEndSubExp = true;
			}
		}

		if(isEndSubExp)
		{
			double valueOfSubExp = ValueStep(container, subExpBeg, subExpEnd);
			list<ExpressPart*>::iterator it_del;
			for(list<ExpressPart*>::iterator it_temp=subExpBeg; it_temp!=subExpEnd; it_temp++)
			{
				it_del = it_temp;
				it_temp--;
				delete *it_del;
				*it_del = NULL;
				container.erase(it_del);
			}
			container.insert(subExpEnd, new ExpNumber(valueOfSubExp));
			isEndSubExp = false;
		}
	}

	vector<double> parameter;
	ExpNumber *ptr_num = NULL;
	for(list<ExpressPart*>::iterator it_loop=next_iterator(it_opt); it_loop != previous_iterator(ExpEnd); it_loop++)
	{
		ptr_num = dynamic_cast<ExpNumber*>(*it_loop);
		parameter.push_back(ptr_num->Value());
	}

	ExpOperator *ptr_opt = dynamic_cast<ExpOperator*>(*it_opt);
	operator_functor *function_ptr = ptr_opt->GetFunctor();

	return (*function_ptr)(parameter);
}

list<ExpressPart*>::iterator Computor::previous_iterator(list<ExpressPart*>::iterator iter)
{
	list<ExpressPart*>::iterator temp_it=iter;
	return --temp_it;
}

list<ExpressPart*>::iterator Computor::next_iterator(list<ExpressPart*>::iterator iter)
{
	list<ExpressPart*>::iterator temp_it=iter;
	return ++temp_it;
}

} // namespace zhcosin
