#ifndef _ERROR_MANAGER_H_
#define _ERROR_MANAGER_H_

#include "includes.h"
#include "Singleton.h"

namespace zhcosin
{

using namespace std;

class ErrorManager : public Singleton<ErrorManager>
{
	DECLARE_SINGLETON(ErrorManager)
private:
	ErrorManager(){}
public:
	~ErrorManager(){}

public:
	bool IsErrorOccur()const
	{
		return !errInfo.empty();
	}
	string GetLastError()const
	{
		return errInfo;
	}
    bool PushErrorInfo(const string &msg)
	{
		errInfo = msg;
		return true;
	}
	bool ClearErrorInfo()
	{
		errInfo.clear();
		return true;
	}
protected:
private:
	string errInfo;
};

} // namespace
#endif
