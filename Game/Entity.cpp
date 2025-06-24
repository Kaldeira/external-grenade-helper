#include "Entity.h"
#include "../Controller/Memory.h"
#include "SDK/SDK.h"

CEntity::CEntity()
{
	pMemory = CMemory::GetInstance();
	pGlobals = CGlobals::GetInstance();
	pSDK = CSDK::GetInstance();
	pOffset = COffsets::GetInstance();
}

bool CEntity::UpdatePointers()
{
	pGlobals->dwGlobalVars = pMemory->read<uint64_t>( pGlobals->clientAddr + pOffset->baseOffsets[ "client.dll" ][ "dwGlobalVars" ] );

	//printf( "dwGlobalVars %p %p\n", pGlobals->dwGlobalVars, pGlobals->clientAddr );

	if ( !pGlobals->dwGlobalVars )
		return false;

	if ( pSDK->IsInGame() )
	{
		if ( !pGlobals->dwEntityList ) {
			pGlobals->dwEntityList = pMemory->read<uint64_t>( pGlobals->clientAddr + pOffset->baseOffsets[ "client.dll" ][ "dwEntityList" ] );
			return false;
		}

		if ( pGlobals->szMapName.empty() )
			pGlobals->szMapName = pSDK->GetGlobalVars().get_current_map_name();

		return true;
	}
	else
	{
		pGlobals->dwEntityList = 0;
		pGlobals->szMapName = {};
	}

	return false;
}

bool CEntity::UpdateLocalPlayer()
{
	if ( !UpdatePointers() ) {
		localPlayer = {};

		return false;
	}

	int highestIndex = I::EntityList.get_highest_entity_index();

	for ( int i = 0; i < highestIndex; i++ )
	{
		uint64_t entityController = I::EntityList.get_entity_controller( i );

		if ( !entityController )
			continue;

		bool isLocal = pMemory->read<bool>( entityController + pOffset->clientOffsets[ "CBasePlayerController" ][ "m_bIsLocalPlayerController" ] );

		if ( isLocal )
		{
			auto getEntity = I::EntityList.get_enitty( entityController );

			if ( !getEntity )
				continue;

			auto className = I::EntityList.get_entity_class_name( entityController );

			auto playerPawn = I::EntityList.get_player_pawn( entityController );

			if ( !playerPawn )
				continue;

			auto cameraServices = pMemory->read<uintptr_t>( playerPawn + pOffset->clientOffsets[ "C_BasePlayerPawn" ][ "m_pCameraServices" ]  );

			if ( !cameraServices )
				continue;

			auto currentWeapon = I::EntityList.get_entity_weapon( playerPawn );
			auto itemDefinitionIndex = pMemory->read<uint32_t>( currentWeapon + pOffset->clientOffsets[ "C_EconEntity" ][ "m_AttributeManager" ]  + pOffset->clientOffsets[ "C_AttributeContainer" ][ "m_Item" ]  + pOffset->clientOffsets[ "C_EconItemView" ][ "m_iItemDefinitionIndex" ] );

			localPlayer.m_vecOrigin = pMemory->read<Vector3>( playerPawn + pOffset->clientOffsets[ "C_BasePlayerPawn" ][ "m_vOldOrigin" ] );
			localPlayer.m_dwEntity = playerPawn;
			localPlayer.m_dwActiveWeapon = currentWeapon;
			localPlayer.m_iItemIndex = itemDefinitionIndex;
			localPlayer.m_vecEyePos = localPlayer.m_vecOrigin + pMemory->read<Vector3>( playerPawn + pOffset->clientOffsets["C_BaseModelEntity"]["m_vecViewOffset"]);
			localPlayer.m_mViewMatrix = pMemory->read<D3DMATRIX>( pGlobals->clientAddr + pOffset->baseOffsets[ "client.dll" ][ "dwViewMatrix" ]);
			localPlayer.m_vecAimPunch = pMemory->read<Vector3>( playerPawn + pOffset->clientOffsets[ "C_CSPlayerPawn" ][ "m_aimPunchAngle" ] );
			localPlayer.m_iFOVStart = pMemory->read<int>( cameraServices + pOffset->clientOffsets[ "CCSPlayerBase_CameraServices" ][ "m_iFOVStart" ] );
			localPlayer.m_iShotsFired = pMemory->read<int>( playerPawn + pOffset->clientOffsets[ "C_CSPlayerPawn" ][ "m_iShotsFired" ] );

			//printf( "x: %f, y: %f, z: %f\n", localPlayer.m_vecAimPunch.x, localPlayer.m_vecAimPunch.y, localPlayer.m_vecAimPunch.z );
			//printf( "itemDefinitionIndex %d\n", itemDefinitionIndex );
			//printf( "x: %f, y: %f, z: %f\n", localPlayer.m_vecEyePos.x, localPlayer.m_vecEyePos.y, localPlayer.m_vecEyePos.z );

			break;
		}
	}
}

void CEntity::Run()
{
	while ( true ) {
		this->UpdateLocalPlayer();
		//printf( "test \n" );
		Sleep( 1 );
	}
}