#include "Render.h"

#include "imgui/ImGui.h"
#include "imgui/backends/ImGui_impl_dx11.h"
#include "imgui/backends/ImGui_impl_win32.h"
#include "imgui/ImGui_internal.h"
#include "../Game/NadeHelper.h"
#include "Input.h"

CRender::CRender()
{
	pGlobals = CGlobals::GetInstance();
}

void CRender::DrawDebug()
{
	if ( !bDebug )
		return;

	static int fontSize = 15;
	float y_start = height / 2;

	char buf[ 32 ];
	sprintf( buf, "FPS: %f", ImGui::GetIO().Framerate );
	DrawString( buf, 10, 20, white, 12, false );

	int index = 0;
	DrawString( "--Nade Helper--", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( std::string("Map: ").append(pGlobals->szMapName).c_str(), 10, y_start + ( fontSize * index++ ), white, fontSize, false);
	DrawString( "[F4] Save Jump + Throw", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( "[F5] Save Throw", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( "[F6] Save W + Jump + Throw", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( "[F7] Close Debug Menu", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( "[F8] Update Nade List", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
	DrawString( "[END] End Program", 10, y_start + ( fontSize * index++ ), white, fontSize, false );
}

void CRender::Run()
{
	while ( !bDestroy )
	{
		m_pInput->UpdateKeyStates();

		if ( m_pInput->Click( VK_END ) ) {
			bDestroy = true;
		}

		if ( m_pInput->Click( VK_F7 ) ) {
			bDebug = !bDebug;
		}

		static bool isHidden = true;
		if ( isHidden && GetForegroundWindow() == pGlobals->gameWindow ) {
			ShowWindow( targetWindow, SW_SHOW );
			isHidden = false;
		}
		else if ( !isHidden && GetForegroundWindow() != pGlobals->gameWindow )
		{
			ShowWindow( targetWindow, SW_HIDE );
			isHidden = true;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			if ( !bDestroy )
			{
				CNadeHelper::GetInstance()->DrawNadeHelper();
				CNadeHelper::GetInstance()->HandlerNadeHelper();
				CNadeHelper::GetInstance()->DrawRecoilHelper();

				DrawDebug();
			}
		}
		ImGui::EndFrame();

		constexpr float color[ 4 ] = { 0.f, 0.f, 0.f, 0.f };
		deviceContext->OMSetRenderTargets( 1, &rtView, nullptr );
		deviceContext->ClearRenderTargetView( rtView, color );

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		swapChain->Present( 1, 0 );

		if ( bDestroy )
			DestroyRender();
	}
}

bool CRender::InitD3D()
{
	targetWindow = pGlobals->targetWindow;

	if ( !targetWindow )
	{
		printf( "[!] Failed to get target window handle\n" );
		return false;
	}

	RECT clientRect{}, windowRect{};

	// retrieve window dimensions
	if ( !GetClientRect( targetWindow, &clientRect ) || !GetWindowRect( targetWindow, &windowRect ) )
	{
		printf( "[!] Invalid window (Error Code: %lu)\n", GetLastError() );
		return false;
	}

	width = static_cast< UINT >( clientRect.right );
	height = static_cast< UINT >( clientRect.bottom );

	// enable click through and layered transparency
	auto windowLong = GetWindowLongA( targetWindow, GWL_EXSTYLE );
	if ( !SetWindowLongA( targetWindow, GWL_EXSTYLE, windowLong | WS_EX_LAYERED | WS_EX_TRANSPARENT ) )
	{
		printf( "[!] Failed to set window style (Error Code: %lu)\n", GetLastError() );
		return false;
	}

	if ( !SetLayeredWindowAttributes( targetWindow, RGB( 0, 0, 0 ), 255, LWA_ALPHA ) )
	{
		printf( "[!] Failed to set window transparency (Error Code: %lu)\n", GetLastError() );
		return false;
	}

	// keep the window on top without resizing/moving
	SetWindowPos( targetWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );

	if ( !SetWindowSwapChain() )
	{
		printf( "[!] Failed to set swap chain\n" );
		return false;
	}

	POINT clientScreen{};
	ClientToScreen( targetWindow, &clientScreen );

	MARGINS margins = {
		clientScreen.x - windowRect.left,
		clientScreen.y - windowRect.top,
		clientRect.right,
		clientRect.bottom
	};

	if ( FAILED( DwmExtendFrameIntoClientArea( targetWindow, &margins ) ) )
	{
		printf( "[!] Failed to extend DWM frame\n" );
		return false;
	}

	ShowWindow( targetWindow, SW_SHOW );

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	if ( !ImGui_ImplWin32_Init( targetWindow ) || !ImGui_ImplDX11_Init( d3d11Device, deviceContext ) )
	{
		printf( "[!] Failed to initialize ImGui\n" );
		return false;
	}

	ImGui::StyleColorsDark();

	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	defaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\verdana.ttf", 20.0f, &font_cfg );

	return true;
}


bool CRender::SetWindowSwapChain()
{
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.BufferCount = 1;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	sd.OutputWindow = pGlobals->targetWindow;
	sd.Windowed = true;

	const D3D_FEATURE_LEVEL featureLevel[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevel, 2, D3D11_SDK_VERSION, &sd, &swapChain, &d3d11Device, nullptr, &deviceContext );

	if ( FAILED( hr ) )
	{
		printf( "[!] Failed to create device, result=%lx\n", hr );
		return false;
	}

	hr = swapChain->GetBuffer( NULL, IID_PPV_ARGS( &backBuffer ) );

	if ( FAILED( hr ) )
	{
		printf( "[!] Failed to get buffer, result=%lx\n", hr );
		return false;
	}

	hr = d3d11Device->CreateRenderTargetView( backBuffer, nullptr, &rtView );

	if ( FAILED( hr ) )
	{
		printf( "[!] Failed to create render target, result=%lx\n", hr );
		return false;
	}

	return true;
}

void CRender::DestroyRender()
{
	if ( !bDestroy )
		return;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if ( rtView ) {
		rtView->Release();
		rtView = nullptr;
	}

	if ( swapChain ) {
		swapChain->Release();
		swapChain = nullptr;
	}

	if ( deviceContext ) {
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if ( d3d11Device ) {
		d3d11Device->Release();
		d3d11Device = nullptr;
	}

	DestroyWindow( pGlobals->targetWindow );

	exit( 1 );
}

void CRender::DrawString( const char* text, int x, int y, ImVec4 color, float fontSize, bool centered, bool outline, ImFont* font )
{
	if ( x < 0 || y < 0 || x > width || y > height )
		return;

	if ( font == nullptr )
		font = defaultFont;

	if ( centered )
	{
		ImVec2 textSize = ImGui::CalcTextSize( text, font, fontSize );
		x = x - textSize.x / 2;
		y = y - textSize.y;
	}

	if ( outline )
	{
		ImGui::GetBackgroundDrawList()->AddText( font, fontSize, ImVec2( x, y - 1 ), ImGui::ColorConvertFloat4ToU32( ImVec4( 0, 0, 0, 1 ) ), text );
		ImGui::GetBackgroundDrawList()->AddText( font, fontSize, ImVec2( x, y + 1 ), ImGui::ColorConvertFloat4ToU32( ImVec4( 0, 0, 0, 1 ) ), text );
		ImGui::GetBackgroundDrawList()->AddText( font, fontSize, ImVec2( x + 1, y ), ImGui::ColorConvertFloat4ToU32( ImVec4( 0, 0, 0, 1 ) ), text );
		ImGui::GetBackgroundDrawList()->AddText( font, fontSize, ImVec2( x - 1, y ), ImGui::ColorConvertFloat4ToU32( ImVec4( 0, 0, 0, 1 ) ), text );
	}

	ImGui::GetBackgroundDrawList()->AddText( font, fontSize, ImVec2( x, y ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.x, color.y, color.z, color.w ) ), text );
}


void CRender::DrawLine( int x1, int y1, int x2, int y2, int thickness, ImVec4 color )
{
	ImGui::GetBackgroundDrawList()->AddLine( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.x, color.y, color.z, color.w ) ), thickness );
}

void CRender::DrawCircle( float x, float y, float radius, ImVec4 color )
{
	Vector3 VertexList[ 46 ];

	float angle = 0.f;
	const float angleStep = ( M_PI * 2 ) / 45;

	for ( int i = 0; i <= 45; ++i, angle += angleStep )
	{
		VertexList[ i ].x = x + cosf( angle ) * radius;
		VertexList[ i ].y = y + sinf( angle ) * radius;
		VertexList[ i ].z = 0.f;
	}

	for ( int i = 0; i < 45; ++i )
		DrawLine( VertexList[ i ].x, VertexList[ i ].y, VertexList[ i + 1 ].x, VertexList[ i + 1 ].y, 1, color );
}

void CRender::DrawRect( int x, int y, int w, int h, ImVec4 color, int thickness )
{
	ImGui::GetBackgroundDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.x, color.y, color.z, color.w ) ), 0, 0, thickness );
}

void CRender::DrawDot( float x, float y, float w, float h, ImVec4 color )
{
	DrawRect( x - 1.0f, y - 1.0f, w + 2.0f, h + 2.0f, color );
	DrawRect( x, y, w, h, color );
}