#ifndef _SHELL_H_
#define _SHELL_H_

#include "includes.h"
#include "Singleton.h"

namespace zhcosin
{

using namespace std;

typedef bool (*pfGetCmdTypeFun)(const string &cmd);
typedef void (*pfDoCmdFun)(const string &cmd);


class Shell : public Singleton<Shell>
{
	DECLARE_SINGLETON(Shell)
private:
	Shell(){}
	~Shell(){}

public:

public:
	void InsertMapItem(int cmdId, pfDoCmdFun pfDoFun)
	{
		table[cmdId] = pfDoFun;
	}

	pfDoCmdFun GetFun(int cmdId)
	{
		return table[cmdId];
	}

private:
	map<int, pfDoCmdFun> table;
};
} // namespace

#endif // _SHELL_H_
