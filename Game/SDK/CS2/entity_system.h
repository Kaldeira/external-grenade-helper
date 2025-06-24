#pragma once

#include "base_handle.h"
#include <array>

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64 // 0x3F
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS // 0x8000

class CEntityIdentity {
public:
	std::uint64_t m_pInstance;     // 0x0
	std::uint8_t padding_0[ 0x8 ];  // 0x8
	CHandle m_EHandle;        // 0x10
	std::uint8_t padding_1[ 0x64 ]; // 0x14
};

class CGameEntitySystem
{
public:
	/*CGameEntitySystem::CGameEntitySystem(uint64_t dwEntityList) {
		identityChunks = CMemory::GetInstance()->m_pMemory->read<std::array<CEntityIdentities*, MAX_ENTITY_LISTS>>(dwEntityList + 0x10);
	}*/

	int get_highest_entity_index()
	{
		if ( !CGlobals::GetInstance()->dwEntityList )
			return 0;

		return CMemory::GetInstance()->m_pMemory->read<int>( CGlobals::GetInstance()->dwEntityList + m_pOffsets->baseOffsets[ "client.dll" ][ "dwGameEntitySystem_highestEntityIndex" ]);
	}

	uint64_t get_entity_controller( const std::uint32_t index ) const {
		if ( index >= MAX_TOTAL_ENTITIES - 1 )
			return 0;

		const uint32_t chunk_index = index & 0x7FFF;

		if ( chunk_index >= MAX_TOTAL_ENTITIES - 1 )
			return 0;

		uintptr_t entry = m_pMemory->read<uintptr_t>( CGlobals::GetInstance()->dwEntityList + 0x8 * ( chunk_index >> 9 ) + 0x10 );

		if ( !entry )
			return 0;

		uintptr_t instance = m_pMemory->read<uintptr_t>( entry + 0x78 * ( index & 0x1FF ) );

		if ( !instance )
			return 0;

		return instance;
	}

	uint64_t get_player_pawn( const uint64_t player_controller ) const {
		const auto pawn_handle = m_pMemory->read<CHandle>( player_controller + m_pOffsets->clientOffsets[ "CBasePlayerController" ][ "m_hPawn" ] ); // m_hPawn

		if ( !pawn_handle.is_valid() )
			return 0;

		const uint32_t pawn_index = pawn_handle.get_entry_index();

		if ( pawn_index >= MAX_TOTAL_ENTITIES - 1 )
			return 0;

		uintptr_t pawn_entry = m_pMemory->read<uintptr_t>( CGlobals::GetInstance()->dwEntityList + 0x8 * ( pawn_index >> 9 ) + 0x10 );

		if ( !pawn_entry )
			return 0;


		uintptr_t pawn_instance = m_pMemory->read<uintptr_t>( pawn_entry + 0x78 * ( pawn_handle.index() & 0x1FF ) );

		if ( !pawn_instance )
			return 0;

		return pawn_instance;
	}

	uint64_t get_entity_weapon( const uint64_t player_pawn )
	{
		const auto weapon_services = m_pMemory->read<uint64_t>( player_pawn + m_pOffsets->clientOffsets[ "C_BasePlayerPawn" ][ "m_pWeaponServices" ] );

		if ( !weapon_services )
			return 0;

		const auto weapon_handle = m_pMemory->read<CHandle>( weapon_services + m_pOffsets->clientOffsets[ "CPlayer_WeaponServices" ][ "m_hActiveWeapon" ] ); // m_hPawn

		if ( !weapon_handle.is_valid() )
			return 0;

		const uint32_t weapon_index = weapon_handle.get_entry_index();

		if ( weapon_index >= MAX_TOTAL_ENTITIES - 1 )
			return 0;

		uintptr_t entry = m_pMemory->read<uintptr_t>( CGlobals::GetInstance()->dwEntityList + 0x8 * ( weapon_index >> 9 ) + 0x10 );

		if ( !entry )
			return 0;

		uintptr_t instance = m_pMemory->read<uintptr_t>( entry + 0x78 * ( weapon_handle.index() & 0x1FF ) );

		if ( !instance )
			return 0;


		return instance;
	}

	uint64_t get_enitty( const uint64_t player_controller ) const {
		return CMemory::GetInstance()->m_pMemory->read<uint64_t>( player_controller + 0x10 );
	}

	std::string get_entity_class_name( __int64 controller )
	{
		__int64 v1; // rcx
		__int64 result; // rax
		__int64 v3; // rcx

		v1 = CMemory::GetInstance()->m_pMemory->read<uint64_t>( get_enitty( controller ) + 8i64 );
		result = CMemory::GetInstance()->m_pMemory->read<uint64_t>( v1 + 0x108 );
		result = CMemory::GetInstance()->m_pMemory->read<uint64_t>( result );

		if ( result )
			return CMemory::GetInstance()->read_string( result, 128 );

		return {};
	}

private:

};

namespace I { inline CGameEntitySystem EntityList; }