#include "plugin.h"

class FooPlugin : public IPlugin
{
public:
	virtual const char* const GetName() { return "FooPlugin"; }
	virtual VersionInfo& GetVersion() { return version; }
	virtual void About();
	virtual void Release() 
	{ 
#if __cplusplus > 199711L
#else
		delete this; 
#endif
	}
private:
	VersionInfo version;
};

PLUGINDECL
