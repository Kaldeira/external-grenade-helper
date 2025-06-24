#pragma once
#include "../includes.h"
#include "imgui/imgui.h"

class CRender : public CSingleton<CRender>
{
private:
	class CGlobals* pGlobals = nullptr;

	ID3D11Device* d3d11Device = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11RenderTargetView* rtView = nullptr;

public:
	CRender();
	void Run();
	bool InitD3D();
	bool SetWindowSwapChain();
	void DestroyRender();
	void DrawDebug();

	void DrawString( const char* text, int x, int y, struct ImVec4 color, float fontSize = 12, bool centered = true, bool outline = true, ImFont* font = nullptr );
	void DrawRect( int x, int y, int w, int h, ImVec4 color, int thickness = 1.f );
	void DrawLine( int x1, int y1, int x2, int y2, int thickness, ImVec4 color );
	void DrawDot( float x, float y, float w, float h, ImVec4 color );
	void DrawCircle( float x, float y, float radius, ImVec4 color );

	bool bDestroy = false;
	bool bDebug = true;

	float width;
	float height;

	ImColor white_ = { 255,255,255,200 };
	ImColor white = { 255,255,255,255 };
	ImColor green = { 128,224,0,255 };
	ImColor red = { 255,0,0,255 };

	struct ImFont* defaultFont = nullptr;
	HWND targetWindow = NULL;
};