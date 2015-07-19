#ifndef ADDRESSTRANSLATOR_H_
#define ADDRESSTRANSLATOR_H_

// check for c++11 support
#if __cplusplus > 199711L

#include <unordered_map>
typedef std::unordered_map<int, int> SparseMatrix;

#else

// different compilers support
#ifdef _MSC_VER // vc++

#pragma warning (disable : 4786)  
#include <hash_map>

#elif __GNUC__

#include <ext/hash_map>

namespace std
{
	using namespace __gnu_cxx;
}

#else
// by default let the compiler use this file and if it doesn't exists fallback
#include <hash_map>

#endif

typedef std::hash_map<int, int> SparseMatrix;

#endif

class AddressTranslator
{
public:
    	static void AddAddress(void * oldAddress, void * newAddress);
    	static void * TranslateAddress(void * oldAddress);
    	static void Reset();

private:
	typedef SparseMatrix AddressTable;
    	static AddressTable s_table;
};



#endif
