#ifndef __PLUGINMANAGER_H__

#define __PLUGINMANAGER_H__

#include <vector>
#include "plugin.h"

#if __cplusplus > 199711L

#include <memory>

typedef std::shared_ptr<IPlugin> PLUGIN_PTR;

#else

typedef IPlugin* PLUGIN_PTR;

#endif



class PluginManager
{
public:
	~PluginManager();

	int LoadPluginsAtDir( const char* path );
	
	size_t NumPlugins() const { return plugins.size(); }
	
	PLUGIN_PTR GetPluginAt( size_t index ) { return plugins[index]; }

	void ClearLoadedPlugins();

private:

	std::vector<PLUGIN_PTR> plugins;
};

#endif
