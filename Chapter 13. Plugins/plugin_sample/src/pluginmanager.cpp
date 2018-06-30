#include "pluginmanager.h"
#include <iostream>
#include <string.h>

using std::cerr;
using std::vector;


#if __cplusplus < 201703L




#ifdef _WIN32

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

int loadPluginsAtDir_Windows( const char* path, vector<PLUGIN_PTR>& plugins );


typedef HMODULE H_LIBRARY;

#else  // UNIX

#include <dirent.h>
#include <errno.h>
#include <dlfcn.h>

int loadPluginsAtDir_Unix( const char* path, vector<PLUGIN_PTR>& plugins );

typedef void* H_LIBRARY;


#endif


#if __cplusplus > 199711L

class PluginDeleter
{
public:
	PluginDeleter( IPlugin* plg, H_LIBRARY handler ) : plugin( plg ), lib( handler ) {}
	

	void operator()( IPlugin* )
	{
		delete plugin;
#if _WIN32
		FreeLibrary( lib );
#else
		dlclose( lib );
#endif
				
	}

	IPlugin* plugin;
	H_LIBRARY lib;
};


#else

vector<H_LIBRARY> hlibs;


#endif




#else // c++ 17 onwards

#include <filesystem>
namespace fs = std::filesystem;

#endif



void closeLibraries()
{
#if __cplusplus > 199711L

#else

	for( vector<H_LIBRARY>::iterator it = hlibs.begin(); it != hlibs.end(); ++it )
	{
#if _WIN32
		FreeLibrary( *it );
#else
		dlclose( *it );
#endif
	}
	hlibs.clear();
#endif
}


PluginManager::~PluginManager()
{
	ClearLoadedPlugins();

	closeLibraries();
}



void PluginManager::ClearLoadedPlugins()
{
#if __cplusplus > 199711L
#else
	for( vector<PLUGIN_PTR>::iterator it = plugins.begin(); it != plugins.end(); ++it )
	{
		(*it)->Release();
	}
	plugins.clear();
#endif
}

int PluginManager::LoadPluginsAtDir( const char* path )
{
#if __cplusplus < 201703L


#ifdef _WIN32
	return loadPluginsAtDir_Windows( path, plugins );
#else
	return loadPluginsAtDir_Unix( path, plugins );
#endif

#else

	// since c++ 17 we have a non-specific platform for iterating over directories
	for( auto& p: fs::directory_iterator( path ) )
	{
		fs::path& path = p.path();
#if _WIN32
		if( path.extension() == ".dll" )
#else
		if( path.extension() == ".so" )
#endif
		{
			loadPlugins( path.c_str(), plugins );
		}
	}

#endif

}


void registerPlugin( H_LIBRARY lib, CREATEPLUGIN* creator, vector<PLUGIN_PTR>& plugins )
{
	IPlugin* plugin = (*creator)();
#if __cplusplus > 199711L
	plugins.push_back( PLUGIN_PTR( plugin, PluginDeleter( plugin, lib ) ) );
#else
	plugins.push_back( plugin );
	hlibs.push_back( lib );
#endif
}


#ifdef _WIN32

void loadPlugin( TCHAR* dllname, vector<PLUGIN_PTR>& plugins  )
{
	HMODULE dll = LoadLibrary( dllname );
	if( dll )
	{
		CREATEPLUGIN* createFunction = (CREATEPLUGIN*)GetProcAddress( dll, xstr( PLUGIN_CREATE ) );
		if( createFunction )
		{
			registerPlugin( dll, createFunction, plugins );
		}
		else
		{
			_tprintf( "Create function not found at:%s\n", dllname );
		}
	}
}

/*
typedef struct _WIN32_FIND_DATA {
  DWORD    dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
  DWORD    dwReserved0;
  DWORD    dwReserved1;
  TCHAR    cFileName[MAX_PATH];
  TCHAR    cAlternateFileName[14];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;
*/

int loadPluginsAtDir_Windows( const char* path, vector<PLUGIN_PTR>& plugins )
{
	char winpath[MAX_PATH];
	strcpy( winpath, path );
	strcat( winpath, "\\*" );
	WIN32_FIND_DATA ffd;
	TCHAR wdir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	size_t pathlen = strlen( winpath ) + 1;
#ifdef UNICODE
	size_t convertedChars = 0;
	if( mbstowcs_s( &convertedChars, &wdir[0], MAX_PATH, winpath, pathlen ) )
	{
		return -1;
	}
#else
	strcpy( wdir, winpath );
#endif
	hFind = FindFirstFile(wdir, &ffd);
	if( hFind != INVALID_HANDLE_VALUE )
	{
		TCHAR* filename = &(ffd.cFileName[0]);
		pathlen = strlen( filename );
		if( pathlen > 3 && !strcmp( &(ffd.cFileName[pathlen-3]),"dll" ) )
		{
			loadPlugin( filename, plugins );
		}
		while (FindNextFile(hFind, &ffd) != 0)
		{
			filename = &(ffd.cFileName[0]);
			pathlen = strlen( filename );
			if( pathlen > 3 && !strcmp( &(ffd.cFileName[pathlen-3]),"dll" ) )
			{
				loadPlugin( filename, plugins );
			}		
		}

		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			_tprintf("%i\n",dwError);
		}

		FindClose(hFind);
		return 0;
	}
	else
	{
		return -2;
	}
}

#else

void loadPlugin( const char* soName, vector<PLUGIN_PTR>& plugins )
{
	void* so = dlopen( soName, RTLD_LAZY | RTLD_GLOBAL );
	if( so )
	{
		CREATEPLUGIN* create_plugin = (CREATEPLUGIN*)dlsym( so, xstr( PLUGIN_CREATE ) );
		if( create_plugin )
		{
			registerPlugin( so, create_plugin, plugins );
		}
	}
}

int loadPluginsAtDir_Unix( const char* path, vector<PLUGIN_PTR>& plugins )
{
	typedef struct dirent dirent;

	DIR* dir = opendir( path );

	if( dir )
	{
		errno = 0;
		dirent* current = readdir( dir );
		while( current != 0 )
		{
			if( strcmp( current->d_name, "." ) && strcmp( current->d_name, ".." ) )
			{
				int len = strlen( current->d_name );
				if( len > 3 && !strcmp( &(current->d_name[len - 3]), ".so" ) )
				{
					loadPlugin( current->d_name, plugins );
				}	
			}
			current = readdir( dir );	
		}

		if( errno )
		{
			cerr << "Errors found when reading the directory: " << path << "\n"; 
		}

		closedir( dir );
		return errno;
	}
	else
	{
		cerr << path << " is not a valid directory\n";	
		return -1;
	}

}


#endif
