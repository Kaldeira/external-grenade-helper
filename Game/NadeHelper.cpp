#include "NadeHelper.h"

#include "../Rendering/Render.h"
#include "../Game/SDK/SDK.h"
#include "../Game/Entity.h"
#include "../Rendering/Input.h"

CNadeHelper::CNadeHelper()
{
	pRender = CRender::GetInstance();
	pSDK = CSDK::GetInstance();
	pEntity = CEntity::GetInstance();
	pGlobals = CGlobals::GetInstance();
}

void CNadeHelper::DrawNadeHelper()
{
	const auto currentItem = pEntity->localPlayer.m_iItemIndex;

	if ( !pSDK->IsGrenade( currentItem ) )
		return;

	if ( updateVector ) {
		vecNade = ReadNadeFile();
	}

	if ( vecNade.empty() )
		return;

	updateVector = false;

	//printf( "vec size: %d\n", vecNade.size() );
	//printf( "name: %s\n", pSDK->GetGlobalVars().get_current_map_name().c_str() );

	Vector2 myAngles = pSDK->GetViewAngles();

	for ( int i = 0; i < vecNade.size(); i++ )
	{
		auto helper = vecNade[ i ];

		if ( helper.MapName != pSDK->GetGlobalVars().get_current_map_name() )
			continue;

		if ( currentItem == WeaponIDI::WEAPON_SMOKEGRENADE && helper.Nade != ( "smoke" ) )
			continue;
		else if ( currentItem == WeaponIDI::WEAPON_FLASHBANG && helper.Nade != ( "flash" ) )
			continue;
		else if ( currentItem == WeaponIDI::WEAPON_INCGRENADE && helper.Nade != ( "molotov" ) )
			continue;
		else if ( currentItem == WeaponIDI::WEAPON_MOLOTOV && helper.Nade != ( "molotov" ) )
			continue;
		else if ( currentItem == WeaponIDI::WEAPON_HEGRENADE && helper.Nade != ( "hegrenade" ) )
			continue;

		Vector3 location = helper.Pos;
		Vector2 spotAngle = helper.Angle;
		Vector3 Angles, Screen;

		float distance = location.Distance( pEntity->localPlayer.m_vecOrigin );

		if ( distance < 600 )
		{
			if ( pSDK->WorldToScreen( location, Screen, pEntity->localPlayer.m_mViewMatrix ) ) {
				if ( distance > 10 ) {
					Draw3DCircle( location, 60, 15, pRender->white );
					pRender->DrawString( helper.Name.c_str(), Screen.x, Screen.y, pRender->white_, 13 );
					//pRender->DrawString( helper.Name.c_str(), Screen.x, Screen.y - 10, pRender->white_, 12 );
				}
				else {
					Draw3DCircle( location, 60, 10, pRender->green );
				}
			}

			if ( distance < 10 && isCloseEnough( location, pEntity->localPlayer.m_vecOrigin, 5.f ) )
			{

				AngleVectors( spotAngle, &Angles );
				Angles = Angles * 150 / Angles.Length();
				Angles = Angles + pEntity->localPlayer.m_vecEyePos;

				if ( !pSDK->WorldToScreen( Angles, Screen, pEntity->localPlayer.m_mViewMatrix ) )
					continue;

				float crossDist = Screen.Distance( Vector3( pRender->width / 2, pRender->height / 2, 0 ) );

				/*if ( crossDist < 10 )
				{
					printf( "%f %f | %f %f\n", helper.Angle.x, helper.Angle.y, myAngles.x, myAngles.y );
				}*/

				if ( crossDist < 10 && isCloseEnough( Vector3( helper.Angle.x, helper.Angle.y, 0 ), Vector3( myAngles.x, myAngles.y, 0 ), 0.1f ) ) {

					pRender->DrawCircle( Screen.x, Screen.y, 15, pRender->green );
					pRender->DrawString( helper.Desc.c_str(), Screen.x + 20, Screen.y - 18, pRender->white, 14, false );


					static float size = 30.f;
					pRender->DrawLine( Screen.x - ( size / 2 ), Screen.y, Screen.x + ( size / 2 ), Screen.y, 1.0f, pRender->green );
					pRender->DrawLine( Screen.x, Screen.y - ( size / 2 ), Screen.x, Screen.y + ( size / 2 ), 1.0f, pRender->green );
				}
				else {
					pRender->DrawCircle( Screen.x, Screen.y, 15, pRender->red );
				}


				pRender->DrawLine( pRender->width / 2, pRender->height / 2, Screen.x, Screen.y, 1, pRender->white_ );

				pRender->DrawString( helper.Name.c_str(), Screen.x + 20, Screen.y - 5, pRender->green, 14, false );
			}
		}
	}
}

void CNadeHelper::HandlerNadeHelper()
{
	if ( !pRender->bDebug )
		return;

	if ( m_pInput->Click( VK_F5 ) )
		SaveCurrentNade( "", "Throw" );

	if ( m_pInput->Click( VK_F4 ) )
		SaveCurrentNade( "", "Jump + Throw" );

	if ( m_pInput->Click( VK_F6 ) )
		SaveCurrentNade( "", "W + Jump + Throw" );

	if ( m_pInput->Click( VK_F8 ) )
		updateVector = true;
}

void CNadeHelper::DrawRecoilHelper()
{
	if ( pEntity->localPlayer.m_iFOVStart <= 0 )
		return;

	Vector3 recoil_angles = pEntity->localPlayer.m_vecAimPunch;
	int shotsFired = pEntity->localPlayer.m_iShotsFired;

	auto x = ( pRender->width / 2 );
	auto y = ( pRender->height / 2 );
	auto size = 1;
	x -= ( pRender->width / pEntity->localPlayer.m_iFOVStart * recoil_angles.y );
	y += ( pRender->height / pEntity->localPlayer.m_iFOVStart * recoil_angles.x );

	if ( shotsFired > 1 )
		pRender->DrawDot( x - ( size / 2 ), y - ( size / 2 ), size, size, pRender->white );
}

void CNadeHelper::Draw3DCircle( Vector3 position, float points, float radius, ImVec4 color ) {
	float step = ( float ) M_PI * 2.0f / points;

	for ( int i = 0; i < points; i++ )
	{
		float a1 = i * step;
		float a2 = ( i + 1 ) * step;

		Vector3 start(
			radius * cosf( a1 ) + position.x,
			radius * sinf( a1 ) + position.y,
			position.z
		);

		Vector3 end(
			radius * cosf( a2 ) + position.x,
			radius * sinf( a2 ) + position.y,
			position.z
		);

		Vector3 start2d, end2d;

		if ( !pSDK->WorldToScreen( start, start2d, pEntity->localPlayer.m_mViewMatrix ) || !pSDK->WorldToScreen( end, end2d, pEntity->localPlayer.m_mViewMatrix ) )
			return;

		pRender->DrawLine( start2d.x, start2d.y, end2d.x, end2d.y, 1, color );
	}
};

void CNadeHelper::AngleVectors( const Vector2& angles, Vector3* forward )
{
	float RadY = M_PI / 180.0f * angles.y;
	float RadX = M_PI / 180.0f * angles.x;

	float sp, sy, cp, cy;

	sp = ( float ) sin( RadX );
	cp = ( float ) cos( RadX );

	sy = ( float ) sin( RadY );
	cy = ( float ) cos( RadY );

	forward->x = ( cp * cy );
	forward->y = ( cp * sy );
	forward->z = -sp;
}

bool CNadeHelper::isCloseEnough( const Vector3& a, const Vector3& b, float tolerance ) {
	return std::abs( a.x - b.x ) <= tolerance &&
		std::abs( a.y - b.y ) <= tolerance &&
		std::abs( a.z - b.z ) <= tolerance;
}

bool CNadeHelper::SaveCurrentNade( std::string nadeName, std::string nadeDesc )
{
	if ( !pEntity->localPlayer.m_dwEntity )
		return false;

	const auto current_weapon = pEntity->localPlayer.m_iItemIndex;

	if ( !pSDK->IsGrenade( current_weapon ) )
		return false;

	auto location = pEntity->localPlayer.m_vecOrigin;
	auto angle = pSDK->GetViewAngles();

	GrenadeData nade;
	nade.Pos = location;
	nade.Angle = Vector2( angle.x, angle.y );
	nade.Desc = nadeDesc.c_str();
	nade.MapName = pGlobals->szMapName.c_str();
	nade.Name = pSDK->TranslateGrenade( current_weapon );//nadeName.c_str(); // can't type a name, so user will have to do it manually in json
	nade.Nade = pSDK->TranslateGrenade( current_weapon );
	this->SaveIntoFile( nade );

	return true;
}

void CNadeHelper::SaveIntoFile( GrenadeData grenade )
{
	std::ifstream inputFile( nadeFileName.c_str() );
	if ( inputFile.is_open() ) {
		inputFile >> jsonNadeHelper;
		inputFile.close();
	}

	jsonNadeHelper.push_back( {
		{"MapName", grenade.MapName},
		{"Name", grenade.Name},
		{"Desc", grenade.Desc},
		{"Nade", grenade.Nade},
		{"Pos", {grenade.Pos.x, grenade.Pos.y, grenade.Pos.z}},
		{"Angle", {grenade.Angle.x, grenade.Angle.y}}
		} );

	// Salvar o JSON atualizado de volta ao arquivo
	std::ofstream outputFile( nadeFileName.c_str() );
	outputFile << jsonNadeHelper.dump( 2 );
	outputFile.close();

	updateVector = true;
}

std::vector<GrenadeData> CNadeHelper::ReadNadeFile()
{
	std::ifstream inputFile( nadeFileName.c_str() );
	json jsonNadeHelper;
	if ( inputFile.is_open() ) {
		inputFile >> jsonNadeHelper;
		inputFile.close();
	}
	else {
		return {};
	}

	std::vector<GrenadeData> loadedNadeHelper;

	if ( jsonNadeHelper.empty() ) {
		return {};
	}

	for ( const auto& element : jsonNadeHelper ) {
		GrenadeData grenade;
		grenade.MapName = element[ "MapName" ];
		grenade.Name = element[ "Name" ];
		grenade.Desc = element[ "Desc" ];
		grenade.Nade = element[ "Nade" ];
		grenade.Pos.x = element[ "Pos" ][ 0 ];
		grenade.Pos.y = element[ "Pos" ][ 1 ];
		grenade.Pos.z = element[ "Pos" ][ 2 ];
		grenade.Angle.x = element[ "Angle" ][ 0 ];
		grenade.Angle.y = element[ "Angle" ][ 1 ];
		loadedNadeHelper.push_back( grenade );
	}

	/*for (const auto& grenade : loadedNadeHelper) {
		std::cout << "MapName: " << grenade.MapName << ", Name: " << grenade.Name << ", Desc: " << grenade.Desc
			<< ", Nade: " << grenade.Nade << ", Pos: (" << grenade.Pos.x << ", " << grenade.Pos.y << ", " << grenade.Pos.z
			<< "), Angle: (" << grenade.Angle.x << ", " << grenade.Angle.y << ")\n";
	}*/

	return loadedNadeHelper;
}

bool CNadeHelper::DeleteFromFile( std::string nadeName )
{
	std::ifstream inputFile( nadeFileName.c_str() );
	if ( inputFile.is_open() ) {
		inputFile >> jsonNadeHelper;
		inputFile.close();
	}
	else
		return false;

	// Remove a specific element by Name
	std::string nameToRemove = nadeName.c_str();
	removeElement( jsonNadeHelper, nameToRemove );

	// Save the updated JSON back to the file
	std::ofstream outputFile( nadeFileName.c_str() );
	outputFile << jsonNadeHelper.dump( 2 );
	outputFile.close();

	return true;
}