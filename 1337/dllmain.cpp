#include <includes.h>
#include <no_renderer/FW1FontWrapper/FW1FontWrapper.h>
#include <no_renderer/no_menu/no_menu.h>
#include <no_renderer/no_gui/no_gui.h>
#include <no_define.h>
#include <fstream>
#include <no_cheat/no_spoofcall.h>
#include <no_cheat/framework.h>
#include <no_cheat/Esp.h>
#include <no_cheat/Aimbot.h>
#include <no_cheat/Exploits.h>
#include <no_cheat/PawnsLoop.h>

// It's generally a good idea to use "using" statements to make your code more readable
using std::ifstream;
using std::ofstream;
using std::ios;
using std::string;

// Use the HRESULT type rather than defining your own function pointer types
typedef HRESULT(*PresentFn)(IDXGISwapChain*, UINT, UINT);
inline PresentFn original_present{ };

typedef HRESULT(*ResizeFn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
inline ResizeFn original_resize{ };

// Declare variables as close to their point of use as possible to improve readability
HRESULT(*present_original)(IDXGISwapChain* swapchain, UINT sync, UINT flags) = nullptr;

ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView = NULL;

UINT vps = 1;
ID3D11RasterizerState* rwState;
ID3D11RasterizerState* rsState;
float ScreenCenterX;
float ScreenCenterY;

// Use bool rather than int for boolean variables
bool firstTime = true;

unsigned long RGBAtoHEX(int r, int g, int b)
{
	// Use bit shifting instead of multiplying and adding
	return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

// Use the no_menu::color type rather than defining your own
void functions_line(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.draw_line(x, y, x2, y2, cool_col);
}

void functions_rect(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.draw_rect(x, y, x2, y2, cool_col);
}

void functions_filled_rect(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.fill_rect(x, y, x2, y2, cool_col);
}

// Use a reference to the string type rather than a pointer to char
const wchar_t* CharToWchar(const string& ch)
{
	// Use the size() function rather than strlen()
	const size_t len = ch.size() + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch.c_str(), len);
	return wch;
}

UINT32 ColorToUint32(const no_menu::color color)
{
	UINT32 result =
		(BYTE(color.a) << 24) +
		(BYTE(color.r) << 16) +
		(BYTE(color.g) << 8) +
		BYTE(color.b);
	return result;
}

void functions_text(int x, int y, no_menu::color color, bool center, const string& text) noexcept
{
	if (pFontWrapper) {
		// Use the string type rather than a pointer to char
		const size_t cSize = text.size() + 1;
		wchar_t* wc = new wchar_t[cSize];
		size_t tmp = 0;
		mbstowcs_s(&tmp, wc, cSize, text.c_str(), cSize);

		pFontWrapper->DrawString(pContext, wc, 14, x, y, ColorToUint32(color), FW1_RESTORESTATE);
		delete[] wc;
	}
}
// Use a reference to the string type rather than a pointer to char
void functions_text(int x, int y, no_menu::color color, bool center, const string& text) noexcept
{
	if (pFontWrapper) {
		const size_t cSize = text.size() + 1;
		wchar_t* wc = new wchar_t[cSize];
		size_t tmp = 0;
		mbstowcs_s(&tmp, wc, cSize, text.c_str(), cSize);

		if (center) {
			// Use a helper function to get the text size rather than calling the same code multiple times
			FW1_RECTF textSize = GetTextSize(pFontWrapper, wc, 14);
			pFontWrapper->DrawString(pContext, wc, 14, x - textSize.Right / 2, y, ColorToUint32(color), FW1_RESTORESTATE);
		}
		else {
			pFontWrapper->DrawString(pContext, wc, 14, x, y, ColorToUint32(color), FW1_RESTORESTATE);
		}

		delete[] wc;
	}
}

FW1_RECTF GetTextSize(IFW1Factory* pFontFactory, const wchar_t* text, float fontSize)
{
	IFW1FontWrapper* pFontWrapper;
	pFontFactory->CreateFontWrapper(pDevice, L"Arial", &pFontWrapper);

	FW1_RECTF textRect = { 0 };
	pFontWrapper->MeasureString(pContext, text, fontSize, &textRect, FW1_RESTORESTATE);

	pFontWrapper->Release();

	return textRect;
}
HRESULT PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (firstTime) {
		pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		D3D11_RASTERIZER_DESC rwDesc = {};
		rwDesc.FillMode = D3D11_FILL_SOLID;
		rwDesc.CullMode = D3D11_CULL_NONE;
		pDevice->CreateRasterizerState(&rwDesc, &rwState);

		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		pDevice->CreateRasterizerState(&rsDesc, &rsState);

		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture);
		pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);

		pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 1920;
		viewport.Height = 1080;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		pContext->RSSetViewports(1, &viewport);

		ScreenCenterX = viewport.Width / 2.0f;
		ScreenCenterY = viewport.Height / 2.0f;

		// Initialize the font wrapper
		FW1CreateFactory(FW1_VERSION, &pFontFactory);
		pFontFactory->CreateFontWrapper(pDevice, L"Tahoma", &pFontWrapper);

		firstTime = false;
	}

	if (GetAsyncKeyState(VK_INSERT))
		menu.show();
	else
		menu.hide();

	if (GetAsyncKeyState(VK_F1))
		pawns.menu_loop();
	else
		pawns.hide_menu();

	// Use a helper function to check if the menu is open
	if (IsMenuOpen()) {
		// Set the rasterizer state to draw the menu
		pContext->RSSetState(rwState);

		// Clear the render target to a solid color
		pContext->ClearRenderTargetView(RenderTargetView, ClearColor);

		// Draw the menu
		menu.draw();

		// Present the swap chain
		return present_original(pSwapChain, SyncInterval, Flags);
	}
	else {
	// Set the rasterizer state to draw the game
	pContext->RSSetState(rsState);

	// Clear the render target to a solid color
	pContext->ClearRenderTargetView(RenderTargetView, ClearColor);

	// Draw the game
	game.draw();

	// Present the swap chain
	return present_original(pSwapChain, SyncInterval, Flags);
}

HRESULT ResizeHook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	// Update the render target size and viewport
	RenderTargetTexture->Release();
	RenderTargetView->Release();

	pSwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture);
	pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);

	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	pContext->RSSetViewports(1, &viewport);

	ScreenCenterX = viewport.Width / 2.0f;
	ScreenCenterY = viewport.Height / 2.0f;

	// Call the original resize function
	return original_resize(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

bool IsMenuOpen()
{
	return menu.is_open();
}

// Other functions

void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)())
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2);
	delete[] wc;
}

void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)(), no_menu::color c)
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2, c);
	delete[] wc;
}

void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)(), no_menu::color c, no_menu::color c2)
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2, c, c2);
	delete[] wc;
}

void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)(), no_menu::color c, no_menu::color c2, no_menu::color c3)
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2, c, c2, c3);
	delete[] wc;
}

void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)(), no_menu::color c, no_menu::color c2, no_menu::color c3, bool(*func3)())
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2, c, c2, c3, func3);
	delete[] wc;
}

void no_cheat::pawns_loop::add_separator()
{
	menu.add_separator();
}

void no_cheat::pawns_loop::menu_loop()
{
	menu.show();
}

void no_cheat::pawns_loop::hide_menu()
{
	menu.hide();
}

bool no_cheat::pawns_loop::is_menu_open()
{
	return menu.is_open();
}
