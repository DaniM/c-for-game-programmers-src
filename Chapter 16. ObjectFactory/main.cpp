#include "Factory.h"
#include "GameEntities.h"
#include <iostream>
#include <assert.h>
#include <cctype>

#ifndef _WIN32

int stricmp( const char* s1, const char* s2 )
{
	int  i = 0;
	while( true )
	{
		char c1 = tolower( s1[i] );
		char c2 = tolower( s2[i] );
		if( c1 < c2 )
		{
			return -1;
		}
		else if( c1 > c2 )
		{
			return 1;
		}
		else
		{
			if( c1 == '\0' )
			{
				return 0;
			}
		}
		++i;
	}
}

#endif

int main()
{
    // Register the maker objects
    Factory<GameEntity> factory;
    factory.Register("GameEntity" ,new Creator<GameEntity, GameEntity>);    
    factory.Register("GameCamera" ,new Creator<GameCamera, GameEntity>);

    // Create a few objects and verify they are of the correct type
    GameEntity * pEntity1 = factory.Create("GameEntity");
    assert(!stricmp(pEntity1->GetClassName(), "GameEntity"));

    GameEntity * pEntity2 = factory.Create("GameCamera");
    assert(!stricmp(pEntity2->GetClassName(), "GameCamera"));

    return 0;
}

