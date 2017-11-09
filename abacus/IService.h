#ifndef _SERVICE_H_
#define _SERVICE_H_

namespace zhcosin
{

class IService
{
protected:
	IService(){bRunState = false;}
	virtual ~IService(){}

public:
	virtual int StartService(){bRunState = true; return 0;}
	virtual int StopService(){bRunState = false; return 0;}
	bool IsStarted(){return bRunState;}

protected:
    bool bRunState;
};

}

#endif // _SERVICE_H_

