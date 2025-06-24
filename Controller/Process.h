#include "../Includes.h"

class CProcess : public CSingleton<CProcess>
{
public:
	bool Init(const wchar_t* processName);
	int GetTID( DWORD dwPID );
	DWORD GetPID( const wchar_t* process );
	std::pair<uint64_t, DWORD> GetModule( const wchar_t* moduleName );

	HANDLE processHandle = NULL;
	DWORD processId = NULL;
};

#define m_pProcess CProcess::GetInstance()