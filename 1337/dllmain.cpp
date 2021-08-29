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


//typedef HRESULT(*present_fn)(IDXGISwapChain*, UINT, UINT);
//inline present_fn original_present{ };

//typedef HRESULT(*resize_fn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
//inline resize_fn original_resize{ };





HRESULT(*present_original)(IDXGISwapChain* swapchain, UINT sync, UINT flags) = nullptr;



//rendertarget
ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView = NULL;

UINT vps = 1;
ID3D11RasterizerState* rwState;
ID3D11RasterizerState* rsState;
float ScreenCenterX;
float ScreenCenterY;

bool firstTime = true;




unsigned long RGBAtoHEX(int r, int g, int b)
{
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}


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


const wchar_t* CharToWchar(const char* ch)
{
	const size_t len = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch, len);
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



void functions_text(int x, int y, no_menu::color color, bool center, const char* text) noexcept
{
	if (pFontWrapper) {
		char* yh = (char*)text;
		const size_t cSize = strlen(yh) + 1;
		wchar_t* wc = new wchar_t[cSize];
		size_t tmp = 0;
		mbstowcs_s(&tmp, wc, cSize, yh, cSize);

		pFontWrapper->DrawString(pContext, wc, 14, x, y, ColorToUint32(color), FW1_RESTORESTATE);
	}
}

void functions_get_text_size(const char* text, int& width, int& height) noexcept
{
	int w_i_d_t_h = 0;
	std::string stringArray[80];

	for (int i = 0; i < strlen(text); i++) {
		stringArray[i] = std::string(text);

		if (stringArray[i] == "i" or stringArray[i] == "t" or stringArray[i] == "r") 
			w_i_d_t_h = w_i_d_t_h + 3;
		else 
			w_i_d_t_h = w_i_d_t_h + 7;

	}

	width = w_i_d_t_h;
	height = 13;
}

float functions_get_frametime() noexcept
{
	return 60.f;
}

short functions_oGetAsyncKeyState(int key_to_get) noexcept {
	//return false;
	return No_Discord::GetAsyncKeyState(key_to_get);
	//return LI_FN(GetAsyncKeyState)(key_to_get);
}

auto InitOnce() -> void
{
	
	no_menu::functions.draw_line = functions_line;
	no_menu::functions.draw_rect = functions_rect;
	no_menu::functions.draw_filled_rect = functions_filled_rect;
	no_menu::functions.draw_text = functions_text;
	no_menu::functions.get_text_size = functions_get_text_size;
	no_menu::functions.get_frametime = functions_get_frametime;
	no_menu::functions.get_key_state_discord = functions_oGetAsyncKeyState;
	
}




static int tab = 0;

bool tab1 = false;
bool tab2 = false;
bool tab3 = false;
bool tab4 = false;

std::vector<std::string> AimbotBone = {
	"Head",
	"Neck",
	"Pelvis",
	"Bottom"
};

std::vector<std::string> ESPType = {
	"Box",
	"Cornered",
	"3D Box"
};



HRESULT present_hooked(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (firstTime)
	{
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
		hResult = pFW1Factory->CreateFontWrapper(pDevice, _(L"Tahoma"), &pFontWrapper);
		pFW1Factory->Release();

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
			pDevice->Release();
		}

		InitOnce(); //only once
		gui.init(pSwapChain);

		firstTime = false;
	}

	if (!RenderTargetView)
	{
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);

		if (!pDevice) return false;

		pDevice->GetImmediateContext(&pContext);

		if (!pContext) return false;

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
			pDevice->Release();
		}
	}

	pContext->RSGetViewports(&vps, &viewport);             // offests are on no_define.h file but u dont need to update them for now untin next fn update because i alredy did it for you
	ScreenCenterX = viewport.Width / 2.0f;                // also make sure u are on 1909 this cheat only works there
	ScreenCenterY = viewport.Height / 2.0f;

	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	gui.begin_scene();
	{
		no_menu::window_input(_("UnrealWindow"), _("Fortnite  "));
		                             // ur  menu name
		if (no_menu::begin_window(_("Dildo Rage | 1310's#1310"), no_menu::vec2({ 545, 580 }), VK_INSERT, no_menu::no_menu_window_flags_none))
		{

			if (tab == 1) { tab1 = true; tab2 = false; tab3 = false; tab4 = false; }
			no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab1); if (no_menu::button(_("Aimbot"), no_menu::vec2({ 120, 37 }))) { tab = 1; }

			no_menu::next_column(130, 0);

			if (tab == 2) { tab1 = false; tab2 = true; tab3 = false; tab4 = false; }
			no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab2); if (no_menu::button(_("ESP"), no_menu::vec2({ 120, 37 }))) { tab = 2; }

			no_menu::next_column(130, 0);

			if (tab == 3) { tab1 = false; tab2 = false; tab3 = true; tab4 = false; }
			no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab3); if (no_menu::button(_("Exploits"), no_menu::vec2({ 120, 37 }))) { tab = 3; }

			no_menu::next_column(130, 0);

			if (tab == 4) { tab1 = false; tab2 = false; tab3 = false; tab4 = true; }
			no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab4); if (no_menu::button(_("Misc"), no_menu::vec2({ 120, 37 }))) { tab = 4; }

			no_menu::back_column(390, 50);

			if (tab == 1) no_menu::begin_groupbox(_("Aimbot"), no_menu::vec2({ 513, 480 }));
			if (tab == 2) no_menu::begin_groupbox(_("ESP"), no_menu::vec2({ 513, 480 }));
			if (tab == 3) no_menu::begin_groupbox(_("Exploits"), no_menu::vec2({ 513, 480 }));
			if (tab == 4) no_menu::begin_groupbox(_("Misc"), no_menu::vec2({ 513, 480 }));


			if (tab == 1) {
				no_menu::checkbox(_("Enable Aimbot"), Settings::Aimbot);
				UI_Help::AimbotTab::Aimbot();

				no_menu::separator(10, false);
				no_menu::slider_int(_("Aimbot Fov Circle"), 10.f, 800.f, Settings::AimbotFovCircle);
				UI_Help::AimbotTab::AimbotFov();

				no_menu::separator(10, false);
				no_menu::slider_int(_("Aimbot Smooth"), 0.0f, 10.f, Settings::AimbotSmooth);
				UI_Help::AimbotTab::AimbotSmooth();

				no_menu::separator(10, false);
				no_menu::combobox(_("Aimbot Bone"), AimbotBone, Settings::AimbotBone_Value);
				UI_Help::AimbotTab::AimbotBone();

				no_menu::separator(10, false);
				no_menu::checkbox(_("Aimbot Prediction"), Settings::AimbotPrediction);
				UI_Help::AimbotTab::AimbotPrediction();
			}

			if (tab == 2) {
				no_menu::checkbox(_("Box ESP"), Settings::ESP);
				UI_Help::ESPTab::ESP();

				no_menu::separator(10, false);
				no_menu::combobox(_("ESP Type"), ESPType, Settings::ESPType_Value);

				no_menu::separator(10, false);
				no_menu::checkbox(_("Skeleton"), Settings::ESP_Skeleton);

				no_menu::separator(10, false);
				no_menu::checkbox(_("Lines"), Settings::ESP_Lines);

				no_menu::separator(10, false);
				no_menu::checkbox(_("Player Names"), Settings::ESP_PlayersNames);

				no_menu::separator(13, false);
				no_menu::text_input("Stream Sniper Player", Settings::ESP_StreamSnipePlayer, 20);

				if (Settings::ESP_StreamSnipePlayer != "") Settings::ESP_StreamSnipe = true;
				else if (Settings::ESP_StreamSnipePlayer == "") Settings::ESP_StreamSnipe = false;
			}

			if (tab == 3) {
				no_menu::checkbox(_("Aim While Jumping"), Settings::AimWhileJumping);
				UI_Help::ExploitsTab::AimWhileJumping();

				no_menu::separator(10, false);
				no_menu::checkbox(_("No Weapon Switch Delay"), Settings::NoAnimation);
				UI_Help::ExploitsTab::NoAnimations();

				no_menu::separator(10, false);
				no_menu::checkbox(_("No Spread"), Settings::NoSpread);
				UI_Help::ExploitsTab::NoSpread();

				no_menu::separator(10, false);
				no_menu::checkbox(_("Rapid Fire"), Settings::RapidFire);
				UI_Help::ExploitsTab::RapidFire();

				no_menu::separator(10, false);
				no_menu::checkbox(_("Trigger Bot"), Settings::TriggerBot);
				UI_Help::ExploitsTab::TriggerBot();

				no_menu::separator(10, false);
				no_menu::checkbox(_("AirStuck"), Settings::AirStuck);
				UI_Help::ExploitsTab::AirStuck();

				no_menu::separator(10, false);
				no_menu::checkbox(_("360 Fov"), Settings::Fov360);
				UI_Help::ExploitsTab::Fov360();

				no_menu::separator(10, false);
				no_menu::checkbox(_("Instant Revive"), Settings::InstantRevive);
				UI_Help::ExploitsTab::InstantRevive();
			}


			if (tab == 4) {
				no_menu::checkbox(_("Visible Check"), Settings::VisibleCheck);
				no_menu::separator(10, false);
				no_menu::checkbox(_("Show Fov Circle"), Settings::ShowFovCircle);
				no_menu::separator(10, false);
				no_menu::checkbox(_("Crosshair"), Settings::Crosshair);
				no_menu::separator(10, false);
				if (no_menu::button(_("Webiste"), no_menu::vec2({ 113, 32 }))) {
					system(_("https://vitalcheats.site/"));
				}             // ur web link here
				no_menu::separator(10, false);
				if (no_menu::button(_("New Server"), no_menu::vec2({ 113, 32 }))) {
					system(_("https://discord.gg/mS9Jzg8SRT"));
					// ur discord server link here
				}
			}

			no_menu::end_groupbox();
			no_menu::end_window();
		}

		PawnsLoop();//tick
		if (Settings::ShowFovCircle) {
			gui.draw_circle(gui.get_width() / 2, gui.get_height() / 2, Settings::AimbotFovCircle, { 255, 255, 255, 255 }, 1);
		}

		//gui.draw_cross(gui.get_width() / 2, gui.get_height() / 2, 100, 100, red);

	}
	gui.end_scene();

	if (pFontWrapper)
	{
		                                        //here add whatever u want
		pFontWrapper->DrawString(pContext, _(L""), 255, 255, 255, 0xff0000ff, FW1_RESTORESTATE);
	}

	return present_original(pSwapChain, SyncInterval, Flags);
}

/*
HRESULT resize(IDXGISwapChain* swapchain, const UINT buffer_count, const UINT width, const UINT height, const DXGI_FORMAT new_format, UINT swapchain_flags) {
	pDevice->Release();
	pContext->Release();
	RenderTargetTexture->Release();
	pDevice = nullptr;

	return original_resize(swapchain, buffer_count, width, height, new_format, swapchain_flags);
}
*/



PVOID SpreadCaller = nullptr;
BOOL(*Spread)(PVOID a1, float* a2, float* a3);
BOOL SpreadHook(PVOID a1, float* a2, float* a3)
{
	if (Settings::NoSpread && _ReturnAddress() == SpreadCaller && No_Discord::GetAsyncKeyState(VK_RBUTTON)) {
		return 0;
	}

	return Spread(a1, a2, a3);
}



void ScanFNObjects() {
	FNObjects::GetPlayerName = Framework::Objects::FindObject("/Script/Engine.PlayerState.GetPlayerName");
	std::cout << "GetPlayerName: " << FNObjects::GetPlayerName << "\n";
}

void Init()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stderr);
	freopen("CONOUT$", "w", stdout);


	Uworld = MemoryHelper::PatternScan(signatures::Uworld_Sig);
	Uworld = RVA(Uworld, 7);

	FreeFn = MemoryHelper::PatternScan(signatures::Free_Sig);

	ProjectWorldToScreen = MemoryHelper::PatternScan(signatures::ProjectWorldToScreen_Sig);
	ProjectWorldToScreen = RVA(ProjectWorldToScreen, 5);

	LineOfS = MemoryHelper::PatternScan(signatures::LineOfSightTo_Sig);
	LineOfS = RVA(LineOfS, 5);

	GetNameByIndex = MemoryHelper::PatternScan(signatures::GetNameByIndex_Sig);

	BoneMatrix = MemoryHelper::PatternScan(signatures::BoneMatrix_Sig);
	BoneMatrix = RVA(BoneMatrix, 5);

	if (!No_Discord::module) {
		MessageBoxA(NULL, "Your Discord Overlay is Off!", "Error", MB_ICONERROR);
		exit(0);
	}

	ScanFNObjects();



	uintptr_t presentscene = MemoryHelper::PatternScanW(No_Discord::module, signatures::DiscordPresentScene_sig);

	No_Discord::InsertHook(presentscene, (uintptr_t)present_hooked, (uintptr_t)&present_original);


	uintptr_t addr = MemoryHelper::PatternScan(signatures::Spread_Sig);
	addr = RVA(addr, 5);
	No_Discord::InsertHook(addr, (uintptr_t)SpreadHook, (uintptr_t)&Spread);

	SpreadCaller = (PVOID)(MemoryHelper::PatternScan(signatures::SpreadCaller_sig));

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
	}
	return TRUE;
}
