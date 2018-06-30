#ifndef RTTI_H_
#define RTTI_H_

#include <cassert>
#include <map>
#include <string>
#include <stdarg.h>


class RTTI 
{
  public:
    RTTI(const std::string & className);
    RTTI(const std::string & className, const RTTI & baseRTTI);
    RTTI(const std::string & className, int numParents, ...);
    ~RTTI();

    const std::string & GetClassName() const { return m_className; }
    bool IsExactly(const RTTI & rtti) const { return (this == &rtti); }
    bool DerivesFrom (const RTTI & rtti) const;

  private:
    // Prevent copying
    RTTI( const RTTI & obj );
    RTTI & operator=( const RTTI & obj );

    const std::string m_className;

    int           m_numParents;
    const RTTI ** m_pBaseRTTI;
};


#define RTTI_DECL \
    public: \
        virtual const RTTI & GetRTTI() { return rtti; } \
        static const RTTI rtti;

#define RTTI_ROOT_IMPL(name) \
    const RTTI name::rtti(#name);

#define RTTI_IMPL(name,parent) \
    const RTTI name::rtti(#name, parent::rtti);


class IRTTI
{
public:
	virtual ~IRTTI() {}	

	virtual const std::string& GetClassName() const = 0; 	

	virtual bool IsExactly( const IRTTI& rtti ) const = 0;

	virtual bool DerivesFrom( const IRTTI& rtti ) const = 0;

	virtual bool derivesFrom( const IRTTI* rtti, std::map<const IRTTI*,bool>& checked ) const = 0;
};


template<size_t N>
class XRTTI : public IRTTI
{
public:

	//virtual ~XRTTI(){}
	

	XRTTI( const std::string& class_name, size_t numParents, ... ) : className( class_name )
	{
		va_list v;
		va_start(v,numParents);

		assert( numParents == N && "Wrong number of parents" );	

		for (size_t i=0; i != numParents; ++i) 
		{
		    parents[i] = va_arg(v, const IRTTI*);
		}
		va_end(v);
	}

	virtual const std::string& GetClassName() const { return className; }

	virtual bool IsExactly( const IRTTI& rtti ) const { return this == &rtti; }

	virtual bool DerivesFrom( const IRTTI& rtti ) const
	{
		std::map<const IRTTI*,bool> cache;
		return derivesFrom( &rtti, cache );
	}

	virtual bool derivesFrom( const IRTTI* rtti, std::map<const IRTTI*,bool>& checked ) const
	{
		for( size_t i = 0; i != N; ++i )
		{
			if( parents[i] == rtti )
			{
				return true;
			}
			else
			{
				bool derives = parents[i]->derivesFrom( parents[i], checked );
				checked[parents[i]] = derives;
				if( derives )
				{
					return true;
				}
			}
		}
		return false;

	}

private:

	XRTTI( const XRTTI& obj );

	XRTTI& operator=( const XRTTI& obj );

	std::string className; 

	const IRTTI* parents[N];
};

template<>
class XRTTI<0> : public IRTTI
{
public:
	//virtual ~XRTTI();
	
	XRTTI( const std::string& class_name);

	virtual const std::string& GetClassName() const;

	virtual bool IsExactly( const IRTTI& rtti ) const;

	virtual bool DerivesFrom( const IRTTI& rtti ) const;

	virtual bool derivesFrom( const IRTTI* rtti, std::map<const IRTTI*,bool>& checked ) const;

private:

	XRTTI( const XRTTI& obj );

	XRTTI& operator=( const XRTTI& obj );

	std::string className; 

};

template<>
class XRTTI<1> : public IRTTI
{
public:
	//virtual ~XRTTI();
	
	XRTTI( const std::string& class_name, const IRTTI& parent );

	virtual const std::string& GetClassName( ) const;

	virtual bool IsExactly( const IRTTI& rtti ) const;

	virtual bool DerivesFrom( const IRTTI& rtti ) const;

	virtual bool derivesFrom( const IRTTI* rtti, std::map<const IRTTI*,bool>& checked ) const;

private:

	XRTTI( const XRTTI& obj );

	XRTTI& operator=( const XRTTI& obj );

	std::string className; 

	const IRTTI* parent;
};


#define RTTI_DECL_NO_PARENT \
    public: \
        virtual const IRTTI& GetRTTI() { return rtti; } \
        static const XRTTI<0> rtti;

#define RTTI_DECL_1_PARENT \
    public: \
        virtual const IRTTI& GetRTTI() { return rtti; } \
        static const XRTTI<1> rtti;

#define RTTI_DECL_2_PARENT \
    public: \
        virtual const IRTTI& GetRTTI() { return rtti; } \
        static const XRTTI<2> rtti;

#define RTTI_NO_PARENT_IMPL(name) \
    const XRTTI<0> name::rtti(#name);

#define RTTI_1_PARENT_IMPL(name,parent) \
    const XRTTI<1> name::rtti(#name, parent::rtti);

#define RTTI_2_PARENT_IMPL(name,parent1,parent2) \
    const XRTTI<2> name::rtti(#name, 2, &parent1::rtti, &parent2::rtti);

#endif  // _RTTI_H_

