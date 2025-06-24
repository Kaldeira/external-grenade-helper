#include "SDK.h"
#include "../../Controller/Memory.h"
#include "../../Rendering/Render.h"

CSDK::CSDK()
{
	pMemory = CMemory::GetInstance();
	pGlobals = CGlobals::GetInstance();
	pRender = CRender::GetInstance();
}


bool CSDK::IsInGame()
{
	auto globalVars = GetGlobalVars();

	if ( globalVars.get_current_map_name().size() > 0 )
		return true;

	return false;
}

CGlobalVarsBase CSDK::GetGlobalVars()
{
	if ( !pGlobals->dwGlobalVars )
		return {};

	return pMemory->read<CGlobalVarsBase>( pGlobals->dwGlobalVars );
}

bool CSDK::WorldToScreen( Vector3 position, Vector3& out, D3DMATRIX viewMatrix )
{
	Vector3 vTransformed;
	vTransformed.x = ( float ) ( position.y * viewMatrix.m[ 0 ][ 1 ] ) + ( float ) ( position.x * viewMatrix.m[ 0 ][ 0 ] ) + ( float ) ( position.z * viewMatrix.m[ 0 ][ 2 ] ) + viewMatrix.m[ 0 ][ 3 ];
	vTransformed.y = ( float ) ( position.y * viewMatrix.m[ 1 ][ 1 ] ) + ( float ) ( position.x * viewMatrix.m[ 1 ][ 0 ] ) + ( float ) ( position.z * viewMatrix.m[ 1 ][ 2 ] ) + viewMatrix.m[ 1 ][ 3 ];
	vTransformed.z = ( float ) ( position.y * viewMatrix.m[ 3 ][ 1 ] ) + ( float ) ( position.x * viewMatrix.m[ 3 ][ 0 ] ) + ( float ) ( position.z * viewMatrix.m[ 3 ][ 2 ] ) + viewMatrix.m[ 3 ][ 3 ];

	if ( vTransformed.z < 0.001 )
		return false;

	vTransformed.x *= 1.0 / vTransformed.z;
	vTransformed.y *= 1.0 / vTransformed.z;

	const int width = pRender->width;
	const int height = pRender->height;

	out.x = width / 2.0f + vTransformed.x * ( width / 2.0f );
	out.y = height / 2.0f - vTransformed.y * ( height / 2.0f );

	return true;
}

Vector2 CSDK::GetViewAngles()
{
	return pMemory->read<Vector2>( pGlobals->clientAddr + m_pOffsets->baseOffsets[ "client.dll" ][ "dwViewAngles" ] );
}

bool CSDK::IsGrenade( uint32_t weaponIndex )
{
	if ( weaponIndex == WeaponIDI::WEAPON_SMOKEGRENADE || weaponIndex == WeaponIDI::WEAPON_FLASHBANG || weaponIndex == WeaponIDI::WEAPON_HEGRENADE || weaponIndex == WeaponIDI::WEAPON_MOLOTOV || weaponIndex == WeaponIDI::WEAPON_INCGRENADE )
		return true;

	return false;
}


std::string CSDK::TranslateGrenade( uint32_t weaponIndex )
{
	if ( weaponIndex == WeaponIDI::WEAPON_SMOKEGRENADE )
		return ( "smoke" );
	else if ( weaponIndex == WeaponIDI::WEAPON_FLASHBANG )
		return ( "flash" );
	else if ( weaponIndex == WeaponIDI::WEAPON_HEGRENADE )
		return ( "hegrenade" );
	else if ( weaponIndex == WeaponIDI::WEAPON_MOLOTOV || weaponIndex == WeaponIDI::WEAPON_INCGRENADE )
		return ( "molotov" );

	return {};
}