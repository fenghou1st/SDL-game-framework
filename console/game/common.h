#pragma once

#include <cstdint>

#include <boost/interprocess/sync/named_semaphore.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Exact-width integer types.
Integer types having exactly the specified width.
int8_t			uint8_t			int16_t			uint16_t
int32_t			uint32_t		int64_t			uint64_t

Minimum-width integer types.
Integer types having at least the specified width.
int_least8_t	uint_least8_t	int_least16_t	uint_least16_t
int_least32_t	uint_least32_t	int_least64_t	uint_least64_t

Fastest minimum-width integer types.
Integer types being usually fastest having at least the specified width.
int_fast8_t		uint_fast8_t	int_fast16_t	uint_fast16_t
int_fast32_t	uint_fast32_t	int_fast64_t	uint_fast64_t

Integer types capable of holding object pointers.
These allow you to declare variables of the same size as a pointer.
intptr_t		uintptr_t

Greatest-width integer types.
Types designating integer data capable of representing any value of any
integer type in the corresponding signed or unsigned category.
intmax_t		uintmax_t
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CAutoMutex
{
public:
	CAutoMutex(const char * name)
		: m_semaphore(boost::interprocess::open_or_create, name, 1) { m_semaphore.wait(); }
	~CAutoMutex() { m_semaphore.post(); }
private:
	boost::interprocess::named_semaphore m_semaphore;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename STRUCT, typename MEMBER> size_t GetMemberOffset(MEMBER STRUCT::*pMember)
{
	STRUCT * p = nullptr;
	char * pS = (char *)p;
	char * pM = (char *)&(p->*pMember);
	return pM - pS;
}
