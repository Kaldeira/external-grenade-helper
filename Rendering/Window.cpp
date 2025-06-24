#include "Window.h"
#include "imgui/backends/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT WndProc( HWND window, UINT message, WPARAM wparam, LPARAM lparam )
{
	if ( ImGui_ImplWin32_WndProcHandler( window, message, wparam, lparam ) )
		return true;

	return DefWindowProcA( window, message, wparam, lparam );
}

bool CWindow::SetupWindow( HWND targetWindow ) //criar a propria janela
{
	windowName.reserve( 16u );
	std::generate_n( std::back_inserter( windowName ), 16u, []{
		thread_local std::mt19937_64 mersenne_engine( std::random_device{}( ) );
		std::uniform_int_distribution<> distribution( 97, 122 ); // 'a', 'z'
		return static_cast<uint8_t>( distribution( mersenne_engine ) );
		} );

	WNDCLASSA window_class
	{
		0,
		WndProc,
		0,
		0,
		GetModuleHandleW( nullptr ),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		windowName.c_str()
	};

	if ( !RegisterClassA( &window_class ) )
		return false;

	if ( !targetWindow )
		return false;

	POINT xy = {};

	GetClientRect( targetWindow, &GameRect );
	ClientToScreen( targetWindow, &xy );
	GameRect.left = xy.x;
	GameRect.top = xy.y;


	myWindow = CreateWindowExA( WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, windowName.c_str(), "", WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, GameRect.right, GameRect.bottom, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea( myWindow, &Margin );
	SetWindowLong( myWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );
	ShowWindow( myWindow, SW_SHOW );
	UpdateWindow( myWindow );

	return true;
}