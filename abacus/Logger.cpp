#include "Logger.h"

namespace zhcosin
{

///////////////////////////////////////////////////
Logger::Logger()
{
	logFileName = "abacus.log";
}
Logger::~Logger()
{
}

int Logger::WriteLog(const string &info)
{
	if(info.empty()) return -1;

	FILE *fp = fopen(logFileName.c_str(), "a");
	if(fp == NULL) return -2;

	fwrite(info.c_str(), info.length(), 1, fp);
	fclose(fp);

	return 0;
}
///////////////////////////////////////////////////
LogService::LogService(){}
LogService::~LogService(){}

int LogService::StartService(){return 0;}
int LogService::StopService(){return 0;}

Logger* LogService::GetLogger()
{
	return Logger::GetInstance();
}

} // namespace
