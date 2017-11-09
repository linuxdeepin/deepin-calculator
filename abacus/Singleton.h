#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace zhcosin
{

#define DECLARE_SINGLETON(type) \
	friend class Singleton<type>;

template<class T>
class Singleton
{
protected:
	Singleton(){}
	virtual ~Singleton(){}

public:
	static T *GetInstance()
	{
		if(!pInstance) pInstance = new T;
		return pInstance;
	}
	static void DestroyInstance()
	{
		if(pInstance)
		{
			delete pInstance;
		}
	}

protected:
	static T *pInstance;
};

template<class T>
T* Singleton<T>::pInstance = NULL;

}
#endif // _SINGLETON_H_

