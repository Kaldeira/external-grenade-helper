#pragma once
#include "../Includes.h"

class CMemory : public CSingleton<CMemory>
{
public:
	CMemory();
	bool ConnectToDriver();
	bool ReadTo( unsigned long long address, void* buffer, size_t size );
	std::string read_string( const std::uintptr_t address, std::size_t size );

	template <class T = uintptr_t>
	T read( uintptr_t address )
	{
		T buffer{};

		if ( ReadTo( address, &buffer, sizeof( T ) ) )
			return buffer;

		return {};

	}

private:
	class CProcess* pProcess = nullptr;
};

#define m_pMemory CMemory::GetInstance()