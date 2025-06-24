#include "Main.h"
#include "Controller/Process.h"
#include "Controller/Memory.h"
#include "Game/Entity.h"
#include "Rendering/Render.h"
#include "Game/SDK/Offsets.h"

bool Initialize()
{
	if ( !m_pOffsets->ReadOffsets() )
		return false;

	if ( !m_pProcess->Init( L"cs2.exe" ) )
		return false;

	m_pGlobals->clientAddr = ( uint64_t ) m_pProcess->GetModule( L"client.dll" ).first;

	if ( !m_pGlobals->clientAddr )
		return false;

	//getting nvidia overlay window to draw above
	m_pGlobals->targetWindow = FindWindowA( "CEF-OSC-WIDGET", "NVIDIA GeForce Overlay" );

	if ( !m_pGlobals->targetWindow )
		return false;

	m_pGlobals->gameWindow = FindWindowA( nullptr, "Counter-Strike 2" );

	if ( !m_pGlobals->gameWindow )
		return false;

	return true;
}

int main()
{
	while ( !Initialize() )
	{
		printf( "[+] Waiting for process... \n" );
		Sleep( 500 );
	}

	printf( "[+] Process Found: \n" );
	printf( "[+] processId: %d \n", m_pProcess->processId );
	printf( "[+] clientAddr: %p \n", m_pGlobals->clientAddr );
	printf( "[+] targetWindow: %p \n", m_pGlobals->targetWindow );
	printf( "[+] gameWindow: %p \n", m_pGlobals->gameWindow );

	std::thread updateEntity( &CEntity::Run, CEntity::GetInstance() );

	auto pRender = CRender::GetInstance();

	if ( pRender->InitD3D() )
	{
		pRender->Run();
	}

	updateEntity.join();

	return 0;
}