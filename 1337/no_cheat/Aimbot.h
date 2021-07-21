#pragma once

auto GetSyscallIndex(std::string ModuleName, std::string SyscallFunctionName, void* Function) -> bool
{
	auto ModuleBaseAddress = LI_FN(GetModuleHandleA)(ModuleName.c_str());
	if (!ModuleBaseAddress)
		ModuleBaseAddress = LI_FN(LoadLibraryA)(ModuleName.c_str());
	if (!ModuleBaseAddress)
		return false;

	auto GetFunctionAddress = LI_FN(GetProcAddress)(ModuleBaseAddress, SyscallFunctionName.c_str());
	if (!GetFunctionAddress)
		return false;

	auto SyscallIndex = *(DWORD*)((PBYTE)GetFunctionAddress + 4);

	*(DWORD*)((PBYTE)Function + 4) = SyscallIndex;

	return true;
}

extern "C"
{
	NTSTATUS _NtUserSendInput(UINT a1, LPINPUT Input, int Size);
};

VOID send_mouse_input(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo)
{
	static bool doneonce;
	if (!doneonce)
	{
		if (!GetSyscallIndex(_("win32u.dll"), _("NtUserSendInput"), _NtUserSendInput))
			return;
		doneonce = true;
	}

	INPUT Input[3] = { 0 };
	Input[0].type = INPUT_MOUSE;
	Input[0].mi.dx = dx;
	Input[0].mi.dy = dy;
	Input[0].mi.mouseData = dwData;
	Input[0].mi.dwFlags = dwFlags;
	Input[0].mi.time = 0;
	Input[0].mi.dwExtraInfo = dwExtraInfo;

	_NtUserSendInput((UINT)1, (LPINPUT)&Input, (INT)sizeof(INPUT));
}

bool InitializeAimbot(uintptr_t CurrentPawn) {

	uintptr_t LocalPlayerState = read<uintptr_t>(LocalPawn + offsets::PlayerState); if (!LocalPlayerState) return false;
	uintptr_t LocalTeamIndex = read<uintptr_t>(LocalPlayerState + offsets::TeamIndex); if (!LocalTeamIndex) return false;
	uintptr_t CurrentPawnPlayerState = read<uintptr_t>(CurrentPawn + offsets::PlayerState); if (!CurrentPawnPlayerState) return false;
	uintptr_t CurrentPawnTeamIndex = read<uintptr_t>(CurrentPawnPlayerState + offsets::TeamIndex); if (!CurrentPawnTeamIndex) return false;

	vec3 LookingAtPoint;
	bool IsVisible;
	if (CurrentPawn == LocalPawn) return false;
	if (Settings::VisibleCheck) {
		IsVisible = Framework::Camera::LineOfSightTo((PVOID)PlayerController, (PVOID)CurrentPawn, &LookingAtPoint);
	}

	if (Settings::Aimbot and No_Discord::GetAsyncKeyState(VK_RBUTTON) and (LocalTeamIndex != CurrentPawnTeamIndex)) {
		if (Settings::Fov360) {
			if (Settings::VisibleCheck and IsVisible) {
				auto NewCameraRotation = Framework::Camera::CalculateNewRotation(CurrentPawn, LocalPawnRelativeLocation, Settings::AimbotPrediction);
				Framework::Pawn::SetControlRotation(NewCameraRotation, false);
				if (Settings::TriggerBot and IsVisible) {
					send_mouse_input(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					send_mouse_input(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
			else if (!Settings::VisibleCheck) {

				auto NewRotation = Framework::Camera::CalculateNewRotation(CurrentPawn, LocalPawnRelativeLocation, Settings::AimbotPrediction);
				Framework::Pawn::SetControlRotation(NewRotation, false);
				if (Settings::TriggerBot and IsVisible) {
					send_mouse_input(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					send_mouse_input(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}
		else if (!Settings::Fov360 and Framework::Utils::InFov(CurrentPawn, Settings::AimbotFovCircle, viewport.Width, viewport.Height)) {
			if (Settings::VisibleCheck and IsVisible) {
				auto NewCameraRotation = Framework::Camera::CalculateNewRotation(CurrentPawn, LocalPawnRelativeLocation, Settings::AimbotPrediction);
				Framework::Pawn::SetControlRotation(NewCameraRotation, false);
				if (Settings::TriggerBot and IsVisible) {
					send_mouse_input(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					send_mouse_input(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
			else if (!Settings::VisibleCheck) {

				auto NewRotation = Framework::Camera::CalculateNewRotation(CurrentPawn, LocalPawnRelativeLocation, Settings::AimbotPrediction);
				Framework::Pawn::SetControlRotation(NewRotation, false);
				if (Settings::TriggerBot and IsVisible) {
					send_mouse_input(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					send_mouse_input(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}
	}
	return true;
}