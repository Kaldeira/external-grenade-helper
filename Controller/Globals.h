#include "../Includes.h"


class CGlobals : public CSingleton<CGlobals>
{
public:
	uintptr_t clientAddr = NULL;
	uintptr_t clientSize = NULL;

	//Game
	uintptr_t dwGlobalVars = NULL;
	uintptr_t dwEntityList = NULL;
	std::string szMapName = {};

	HWND targetWindow = NULL;
	HWND gameWindow = NULL;
};

#define m_pGlobals CGlobals::GetInstance()