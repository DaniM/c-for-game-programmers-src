
#include "StdAfx.h"
#include "RTTI.h"


RTTI::RTTI(const std::string & className) : 
    m_className(className), 
    m_numParents(0),
    m_pBaseRTTI(NULL)
{}


RTTI::RTTI(const std::string & className, const RTTI & baseRTTI) :
    m_className(className), 
    m_numParents(1)
{
    m_pBaseRTTI = new const RTTI*[1];
    m_pBaseRTTI[0] = &baseRTTI;
}


RTTI::RTTI(const std::string & className, int numParents, ...) :
    m_className(className)
{
    if (numParents < 1)
    {
        m_numParents = 0;
        m_pBaseRTTI  = NULL;
    }
    else
    {
        m_numParents = numParents;
        m_pBaseRTTI = new const RTTI*[m_numParents];

        va_list v;
        va_start(v,numParents);
        for (int i=0; i < m_numParents; ++i) 
        {
            m_pBaseRTTI[i] = va_arg(v, const RTTI*);
        }
        va_end(v);
    }
}


RTTI::~RTTI()
{
    delete [] m_pBaseRTTI;
}


bool RTTI::DerivesFrom (const RTTI & rtti) const
{
    const RTTI * pCompare = this;
    if (pCompare == &rtti)
        return true;

    for (int i=0; i < m_numParents; ++i)
    {
        if (m_pBaseRTTI[i]->DerivesFrom(rtti))
            return true;
    }

    return false;
}



using std::string;
using std::map;

//XRTTI<0>::~XRTTI() {}
XRTTI<0>::XRTTI( const string& class_name) : className( class_name ) {}

const string& XRTTI<0>::GetClassName() const { return className; }

bool XRTTI<0>::IsExactly( const IRTTI& rtti ) const { return this == &rtti; }

bool XRTTI<0>::DerivesFrom( const IRTTI& rtti ) const { return false; }

bool XRTTI<0>::derivesFrom( const IRTTI* rtti, map<const IRTTI*,bool>& checked ) const { return false; }


//XRTTI<1>::~XRTTI() {}
XRTTI<1>::XRTTI( const string& class_name, const IRTTI& rtti_parent ) : className( class_name ), 
							parent( &rtti_parent ) {}

const string& XRTTI<1>::GetClassName( ) const { return className; }

bool XRTTI<1>::IsExactly( const IRTTI& rtti ) const { return this == &rtti; }

bool XRTTI<1>::DerivesFrom( const IRTTI& rtti ) const 
{
	map<const IRTTI*,bool> cache;
	return derivesFrom( &rtti, cache );
}

bool XRTTI<1>::derivesFrom( const IRTTI* rtti, map<const IRTTI*,bool>& checked ) const
{
	if( rtti == parent )
	{
		return true;
	}
	else
	{
		bool derives = parent->derivesFrom( rtti, checked );
		checked[parent] = derives;
		return derives;
	}
}


typedef XRTTI<0> RTTI_NoParent;
typedef XRTTI<1> RTTI_Parent;

