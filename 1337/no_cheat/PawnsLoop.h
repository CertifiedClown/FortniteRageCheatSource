#pragma once


bool PawnsLoop() {
	try {
		uintptr_t World = read<uintptr_t>(Uworld); if (!World) return false;
		uintptr_t Gameinstance = read<uintptr_t>(World + offsets::OwningGameInstance);  if (!Gameinstance) return false;
		uintptr_t World_Players = read<uintptr_t>(Gameinstance + offsets::LocalPlayers); if (!World_Players) return false;
		uintptr_t World_Player = read<uintptr_t>(World_Players); if (!World_Player)  return false;
		PlayerController = read<uintptr_t>(World_Player + offsets::PlayerController); if (!PlayerController) return false;
		uintptr_t World_PlayerCameraManager = read<uintptr_t>(PlayerController + offsets::PlayerCameraManager); if (!World_PlayerCameraManager) return false;
		FOVAngle = Framework::Camera::GetFOVAngle(World_PlayerCameraManager); Framework::Camera::GetPlayerViewPoint(World_PlayerCameraManager, &CamLoc, &CamRot);
		LocalPawn = read<uintptr_t>(PlayerController + offsets::AcknowledgedPawn);
		uintptr_t PersistentLevel = read<uintptr_t>(World + offsets::PersistentLevel); if (!PersistentLevel) return false;
		uintptr_t AActors = read<uintptr_t>(PersistentLevel + offsets::AActors); if (!AActors) return false;
		uintptr_t ActorCount = read<int>(PersistentLevel + offsets::ActorCount); if (!ActorCount) return false;
		uintptr_t LocalRootComponent; vec3 LocalRelativeLocation;
		if (IsValidPointer(LocalPawn)) {
			LocalRootComponent = read<uintptr_t>(LocalPawn + 0x130); if (!LocalRootComponent) return false;
			LocalRelativeLocation = read<vec3>(LocalRootComponent + 0x11C);
			LocalPawnRelativeLocation = read<vec3>(LocalRootComponent + 0x11C);
		}

		for (int i = 0; i < ActorCount; i++) {
			auto CurrentPawn = read<uintptr_t>(AActors + i * sizeof(uintptr_t));


			auto CurrentPawn_ObjectName = Framework::Objects::GetUintObjectName(CurrentPawn);

			if (strstr(CurrentPawn_ObjectName, "PlayerPawn")) {
				InitializeESP(CurrentPawn);
				if (IsValidPointer(LocalPawn)) {
					InitializeAimbot(CurrentPawn);
					InitializeExploits(CurrentPawn);
				}
			}
			

		}
	}
	catch (...) {}
}