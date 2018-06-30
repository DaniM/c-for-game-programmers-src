#include "fooplugin.h"
#include <iostream>

using std::cout;

void FooPlugin::About()
{
	cout << "Sample plugin: FooPlugin\n";
}

PLUGIN_EXPORT(FooPlugin)
