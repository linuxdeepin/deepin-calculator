#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "includes.h"
#include "IService.h"
#include "Singleton.h"

namespace zhcosin
{

using std::string;

class Logger : public Singleton<Logger>
{
	DECLARE_SINGLETON(Logger)
private:
	Logger();
	~Logger();

public:
	int WriteLog(const string &info);
private:
	string logFileName;
};

////////////////////////////////////////////////
class LogService : public IService,
		   public Singleton<LogService>
{
	DECLARE_SINGLETON(LogService)
private:
	LogService();
	~LogService();

public:
	int StartService();
	int StopService();

public:
	Logger *GetLogger();
private:
};

}

#endif	// _LOGGER_H_

