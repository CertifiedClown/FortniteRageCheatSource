#pragma once
#define RVA(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))
#define M_PI	3.14159265358979323846264338327950288419716939937510

//Sigs Addresses
uintptr_t Uworld;
uintptr_t FreeFn;
uintptr_t GetNameByIndex;
uintptr_t LineOfS;
uintptr_t BoneMatrix;
uintptr_t ProjectWorldToScreen;

//Read Addresses
uintptr_t PlayerController;
uintptr_t LocalPawn;
vec3 LocalPawnRelativeLocation;


float FOVAngle;
vec3 CamLoc;
vec3 CamRot;

namespace signatures {
	const char* Uworld_Sig = "48 8B 05 ? ? ? ? 4D 8B C2"; //uworld

	const char* Gobject_Sig = "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1";

	const char* Free_Sig = "48 85 C9 0F 84 ? ? ? ? 53 48 83 EC 20 48 89 7C 24 30 48 8B D9 48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? 48 8B 07 4C 8B 40 30 48 8D 05 ? ? ? ? 4C 3B C0"; //Free

	const char* ProjectWorldToScreen_Sig = "E8 ? ? ? ? 41 88 07 48 83 C4 30"; //ProjectWorldToScreen

	const char* LineOfSightTo_Sig = "E8 ? ? ? ? 48 8B 0D ? ? ? ? 33 D2 40 8A F8"; //LineOfSight

	const char* GetNameByIndex_Sig = "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 48 8B F2 44 39 71 04 0F 85 ? ? ? ? 8B 19 0F B7 FB E8 ? ? ? ? 8B CB 48 8D 54 24"; //GetNameByIndex

	const char* BoneMatrix_Sig = "E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"; //BoneMatrix

	const char* Spread_Sig = "E8 ? ? ? ? 48 8D 4B 28 E8 ? ? ? ? 48 8B C8"; //Spread

	const char* SpreadCaller_sig = "0F 57 D2 48 8D 4C 24 ? 41 0F 28 CC E8 ? ? ? ? 48 8B 4D B0 0F 28 F0 48 85 C9"; //SpreadCaller

	const char* DiscordPresentScene_sig = "56 57 53 48 83 EC 30 44 89 C6"; //Discord PresentScene
}
	
namespace offsets {
	uintptr_t OwningGameInstance = 0x180;
	uintptr_t LocalPlayers = 0x38;
	uintptr_t PlayerController = 0x30;
	uintptr_t PlayerCameraManager = 0x2B8;
	uintptr_t AcknowledgedPawn = 0x2A0;

	uintptr_t Levels = 0x138;
	uintptr_t PersistentLevel = 0x30;
	uintptr_t AActors = 0x98;
	uintptr_t ActorCount = 0xA0;

	uintptr_t ComponentVelocity = 0x140;
	uintptr_t RootComponent = 0x130;
	uintptr_t FireStartLoc = 0x878;
	uintptr_t RelativeLocation = 0x11c;
	uintptr_t RelativeRotation = 0x128;
	uintptr_t CurrentWeapon = 0x410;
	uintptr_t PlayerState = 0x240;
	uintptr_t Mesh = 0x280;
	uintptr_t TeamIndex = 0xED0;


	//Exploits
	uintptr_t bDisableEquipAnimation = 0x2B4;
	uintptr_t bADSWhileNotOnGround = 0x3B74;
	uintptr_t ReviveFromDBNOTime = 0x3420;
	uintptr_t CustomTimeDilation = 0x538;
	uintptr_t LastFireTime = 0x9BC;
	uintptr_t LastFireTimeVerified = 0x9C0;//0x9C0;

}

namespace FNObjects {
	PVOID GetPlayerName;
}

D3D11_VIEWPORT viewport;
no_gui gui;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

IFW1Factory* pFW1Factory = NULL;
IFW1FontWrapper* pFontWrapper = NULL;

namespace Settings
{
	bool Aimbot = false;
	int AimbotFovCircle = 100;
	int AimbotSmooth = 0.0f;
	static int AimbotBone_Value = 0;
	bool AimbotPrediction = false;


	bool ESP = false;
	static int ESPType_Value = 1;
	bool ESP_Skeleton = false;
	bool ESP_Lines = false;
	bool ESP_PlayersNames = false;
	bool ESP_StreamSnipe = false;
	std::string ESP_StreamSnipePlayer = "";

	bool ShowFovCircle = true;
	bool Variable_For_test = false;

	//Exploits

	bool AimWhileJumping = false;
	bool NoAnimation = false;
	bool NoSpread = false;
	bool RapidFire = false;
	bool TriggerBot = false;
	bool AirStuck = false;
	bool Fov360 = false;
	bool InstantRevive = false;


	
	
	
	bool VisibleCheck = false;
	
	
	
	bool Crosshair = false;
	bool DebugForTest = false;
}

namespace UI_Help {
	namespace AimbotTab {
		void Aimbot() {
			no_menu::SetWidgetPosition(129, 58);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Memory Aimbot] Locks your aim to the player");
			}

			no_menu::SetWidgetPosition(8, 75);
		}

		void AimbotFov() {
			no_menu::SetWidgetPosition(167, 96);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Aimbot Fov] The size of the circle where the Aimbot should lock");
			}

			no_menu::SetWidgetPosition(8, 120);
		}

		void AimbotSmooth() {
			no_menu::SetWidgetPosition(167, 140);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Aimbot Smooth] The time that the camera takes to lock the player");
			}

			no_menu::SetWidgetPosition(8, 164);
		}

		void AimbotBone() {
			no_menu::SetWidgetPosition(118, 183);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Aimbot Bone] You can choose on what bone the Aimbot should lock");
			}

			no_menu::SetWidgetPosition(8, 214);
		}

		void AimbotPrediction() {
			no_menu::SetWidgetPosition(159, 222);
			
			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Aimbot Prediction BETA] It predicts the player movement");
			}
		}
	}


	namespace ESPTab {
		void ESP() {
			no_menu::SetWidgetPosition(107, 58);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[ESP] You can see Players, Objects, Loot through walls");
			}

			no_menu::SetWidgetPosition(8, 78);
		}
	}

	namespace ExploitsTab {
		void AimWhileJumping() {
			no_menu::SetWidgetPosition(154, 59);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Aim While Jumping] You can Aim while you are jumping");
			}

			no_menu::SetWidgetPosition(8, 74);
		}

		void NoAnimations() {
			no_menu::SetWidgetPosition(124, 82);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[No Animation] It skips your switch weapon animation");
			}

			no_menu::SetWidgetPosition(8, 99);
		}

		void NoSpread() {
			no_menu::SetWidgetPosition(96, 107);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[No Spread] It reduces the bullet spread on every weapon");
			}

			no_menu::SetWidgetPosition(8, 126);
		}

		void RapidFire() {
			no_menu::SetWidgetPosition(102, 134);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Rapid Fire] It increase the fire rate of some weapons");
			}

			no_menu::SetWidgetPosition(8, 153);


		}

		void TriggerBot() {
			no_menu::SetWidgetPosition(110, 161);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Trigger Bot] Automatically shoot when a player is visible");
			}

			no_menu::SetWidgetPosition(8, 181);

		}

		void AirStuck() {
			no_menu::SetWidgetPosition(88, 189);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[AirStuck] Your player will be frozen and stuck in air when pressing ALT");
			}

			no_menu::SetWidgetPosition(8, 208);
		}

		void Fov360() {
			no_menu::SetWidgetPosition(80, 216);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[360 Fov] The Aimbot will lock even on players behind you");
			}

			no_menu::SetWidgetPosition(8, 235);
		}

		void InstantRevive() {
			no_menu::SetWidgetPosition(129, 243);

			if (no_menu::TextHoverable("(?)", no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Instant Revive] It sets the revive time to 0 for your team");
			}
		}
	}

}
