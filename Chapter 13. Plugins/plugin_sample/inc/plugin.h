#ifndef __PLUGIN_H__

#define __PLUGIN_H__

#include <string>


#if _WIN32

#define PLUGINDECL_EXPORT __declspec(dllexport)

#define PLUGINDECL_IMPORT __declspec(dllimport)

#else

#define PLUGINDECL_EXPORT

#define PLUGINDECL_IMPORT

#endif


#ifdef DLL

#define API PLUGINDECL_EXPORT

#else

#define API PLUGINDECL_IMPORT

#endif


struct VersionInfo
{
	std::string Version;
};

class IPlugin
{
public:
	virtual ~IPlugin(){}
	virtual const char* const GetName() = 0;
	virtual VersionInfo& GetVersion() = 0;
	virtual void About() = 0;
	virtual void Release() = 0;
};


#define xstr(s) str(s)
#define str(s) #s

#define PLUGIN_CREATE CreatePlugin

#define PLUGIN_EXPORT(name) \
extern "C" PLUGINDECL_EXPORT IPlugin* PLUGIN_CREATE(){\
	return new name();\
}

#define PLUGINDECL \
extern "C" API IPlugin* PLUGIN_CREATE();

typedef PLUGINDECL_IMPORT IPlugin* CREATEPLUGIN();

#endif
