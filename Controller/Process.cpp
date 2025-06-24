#include "Process.h"
#include <Psapi.h>

bool CProcess::Init( const wchar_t* processName )
{
	this->processId = this->GetPID( processName );

	if ( !this->processId )
		return false;

	this->processHandle = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->processId );
	if ( !this->processHandle ) {
		std::cerr << "Erro ao abrir processo.\n";
		return false;
	}

	return true;
}

int CProcess::GetTID( DWORD dwPID )
{
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( hProcessSnap == INVALID_HANDLE_VALUE )
		return 0;

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof( pe32 );
	BOOL bRet = ::Process32First( hProcessSnap, &pe32 );;
	while ( bRet )
	{
		if ( pe32.th32ProcessID == dwPID )
		{
			::CloseHandle( hProcessSnap );
			return pe32.cntThreads;
		}
		bRet = ::Process32Next( hProcessSnap, &pe32 );
	}
	return 0;
}

DWORD CProcess::GetPID( const wchar_t* process )
{
	DWORD dwRet = 0;
	DWORD dwThreadCountMax = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	Process32First( hSnapshot, &pe32 );
	do
	{
		if ( _wcsicmp( pe32.szExeFile, process ) == 0 )
		{
			DWORD dwTmpThreadCount = GetTID( pe32.th32ProcessID );

			if ( dwTmpThreadCount > dwThreadCountMax )
			{
				dwThreadCountMax = dwTmpThreadCount;
				dwRet = pe32.th32ProcessID;
			}
		}
	} while ( Process32Next( hSnapshot, &pe32 ) );
	CloseHandle( hSnapshot );

	return dwRet;
}

std::pair<uint64_t, DWORD> CProcess::GetModule( const wchar_t* moduleName ) {
	if ( this->processHandle == NULL ) {
		std::cerr << "Falha ao abrir processo. Error code: " << GetLastError() << std::endl;
		return {};
	}

	HMODULE hMods[ 1024 ];
	DWORD cbNeeded;
	if ( EnumProcessModules( this->processHandle, hMods, sizeof( hMods ), &cbNeeded ) ) {
		for ( unsigned int i = 0; i < ( cbNeeded / sizeof( HMODULE ) ); i++ ) {
			TCHAR szModName[ MAX_PATH ];
			if ( GetModuleFileNameEx( this->processHandle, hMods[ i ], szModName, sizeof( szModName ) / sizeof( TCHAR ) ) ) {

				std::wstring fullPath = szModName;
				size_t pos = fullPath.find_last_of( L"\\/" );
				std::wstring fileName = ( pos != std::wstring::npos ) ? fullPath.substr( pos + 1 ) : fullPath;

				if ( _wcsicmp( fileName.c_str(), moduleName ) == 0 )
				{
					//printf( "found %S\n", fileName.c_str() );
					MODULEINFO mi;
					GetModuleInformation( this->processHandle, hMods[ i ], &mi, sizeof( mi ) );
					return {(uintptr_t)mi.lpBaseOfDll, mi.SizeOfImage};
				}
			}
		}
	}
	return {};
}