#pragma once

#include "../includes.h"

class CWindow : public CSingleton<CWindow>
{
public:
	bool SetupWindow( HWND targetWindow );
	void SetWindowToTarget();

	HWND myWindow;
	std::string windowName;
	RECT GameRect = { NULL };
	const MARGINS Margin = { -1 };
};
