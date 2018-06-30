#include <iostream>
#include "pluginmanager.h"

using namespace std;

int main( int argc, const char* args[] )
{
	PluginManager pMgr;

	if( argc > 1 )
	{
		cout << "Loading plugins at: "<< args[1] << "\n";
		pMgr.LoadPluginsAtDir( args[1] );
	}
	else
	{
		cout << "Loading plugins at current directory\n";
		pMgr.LoadPluginsAtDir( "." );
	}

	cout << "Plugins found: " << pMgr.NumPlugins() << "\n";
	for( size_t i = 0; i != pMgr.NumPlugins(); ++i )
	{
		PLUGIN_PTR plugin = pMgr.GetPluginAt( i );
		plugin->About();
		cout << plugin->GetName() << "\n" ;	
	}
	return 0;
}
