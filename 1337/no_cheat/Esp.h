#pragma once

vec3 head2, neck, pelvis, chest, chesti, chestatright, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

void GetAllBonesPos(uintptr_t CurrentPawn) {
	Framework::Pawn::GetBoneLocation(CurrentPawn, 66, &head2); Framework::Pawn::WorldToScreen(head2, &head2);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 65, &neck); Framework::Pawn::WorldToScreen(neck, &neck);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 2, &pelvis); Framework::Pawn::WorldToScreen(pelvis, &pelvis);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 36, &chesti); Framework::Pawn::WorldToScreen(chesti, &chesti);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 8, &chestatright); Framework::Pawn::WorldToScreen(chestatright, &chestatright);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 9, &leftShoulder); Framework::Pawn::WorldToScreen(leftShoulder, &leftShoulder);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 37, &rightShoulder); Framework::Pawn::WorldToScreen(rightShoulder, &rightShoulder);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 10, &leftElbow); Framework::Pawn::WorldToScreen(leftElbow, &leftElbow);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 38, &rightElbow); Framework::Pawn::WorldToScreen(rightElbow, &rightElbow);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 11, &leftHand); Framework::Pawn::WorldToScreen(leftHand, &leftHand);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 39, &rightHand); Framework::Pawn::WorldToScreen(rightHand, &rightHand);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 67, &leftLeg); Framework::Pawn::WorldToScreen(leftLeg, &leftLeg);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 74, &rightLeg); Framework::Pawn::WorldToScreen(rightLeg, &rightLeg);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 73, &leftThigh); Framework::Pawn::WorldToScreen(leftThigh, &leftThigh);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 80, &rightThigh); Framework::Pawn::WorldToScreen(rightThigh, &rightThigh);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 68, &leftFoot); Framework::Pawn::WorldToScreen(leftFoot, &leftFoot);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 75, &rightFoot); Framework::Pawn::WorldToScreen(rightFoot, &rightFoot);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 71, &leftFeet); Framework::Pawn::WorldToScreen(leftFeet, &leftFeet);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 78, &rightFeet); Framework::Pawn::WorldToScreen(rightFeet, &rightFeet);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 72, &leftFeetFinger); Framework::Pawn::WorldToScreen(leftFeetFinger, &leftFeetFinger);
	Framework::Pawn::GetBoneLocation(CurrentPawn, 79, &rightFeetFinger); Framework::Pawn::WorldToScreen(rightFeetFinger, &rightFeetFinger);
	chest.x = chesti.x + ((chestatright.x - chesti.x) / 2);
	chest.y = chesti.y;
}
/*
void DrawBox(int x, int y, int PawnWidth, int PawnHeight, color color, int BoxType)
{
	if (BoxType == 0) {//Normal Boxes
		x = x - (PawnWidth / 2);

		gui.draw_line(x, y, x + PawnWidth, y, color); //bottom
		gui.draw_line(x, y, x, y + PawnHeight, color); //left
		gui.draw_line(x + PawnWidth, y, x + PawnWidth, y + PawnHeight, color); //right
		gui.draw_line(x, y + PawnHeight, x + PawnWidth, y + PawnHeight, color); //up
	}

	if (BoxType == 1) {//Cornered Boxes
		float lineW = (PawnWidth / 4);
		float lineH = (PawnHeight / 3);

		gui.draw_line(x, y, x, y + lineH, color); //Da in alto a sinistra verso giù
		gui.draw_line(x, y, x + lineW, y, color); //Da in alto a sinistra verso destra
		gui.draw_line(x + PawnWidth - lineW, y, x + PawnWidth, y, color); //Da al centro in alto a destra verso in alto a destra
		gui.draw_line(x + PawnWidth, y, x + PawnWidth, y + lineH, color); //Da in alto a destra verso giù
		gui.draw_line(x, y + PawnHeight - lineH, x, y + PawnHeight, color); //Dal centro in basso a sinistra verso in basso a sinistra
		gui.draw_line(x, y + PawnHeight, x + lineW, y + PawnHeight, color); //Da in basso a sinitra verso destra
		gui.draw_line(x + PawnWidth - lineW, y + PawnHeight, x + PawnWidth, y + PawnHeight, color); //Dal centro in basso basso a desta verso  in basso a destra
		gui.draw_line(x + PawnWidth, y + PawnHeight - lineH, x + PawnWidth, y + PawnHeight, color); //Dal centro a destra in basso verso in basso a destra
	}



}
*/

inline bool ProcessEvent(uintptr_t address, void* fnobject, void* parms)
{
	UObject* addr = reinterpret_cast<UObject*>(address); if (!addr) return false;
	auto vtable = *reinterpret_cast<void***>(addr); if (!vtable) return false;
	auto processEventFn = static_cast<void(*)(void*, void*, void*)>(vtable[0x68]); if (!processEventFn) return false;
	SpoofCall(processEventFn, (void*)addr, (void*)fnobject, (void*)parms);
	return true;
}


typedef struct {
	float X, Y, Z;
} FVector;



struct UPrimitiveComponent_SetActorEnableCollision_Params
{
	bool                                               bNewActorEnableCollision;                                          // (Parm, ZeroConstructor, IsPlainOldData)
};

void SetActorEnableCollision(uint64_t pawn, bool bNewActorEnableCollision) {
	static PVOID fn = nullptr;

	if (!fn)
		fn = Framework::Objects::FindObject(_("PrimitiveComponent.SetActorEnableCollision"));
	UPrimitiveComponent_SetActorEnableCollision_Params params;
	params.bNewActorEnableCollision = bNewActorEnableCollision;
	ProcessEvent(pawn, fn, &params);
}


// Function Engine.PrimitiveComponent.SetEnableGravity
struct UPrimitiveComponent_SetEnableGravity_Params
{
	bool                                               bGravityEnabled;                                          // (Parm, ZeroConstructor, IsPlainOldData)
};
void SetEnableGravity(uint64_t primitive_component, bool bEnable)
{
	static PVOID fn = nullptr;

	if (!fn)
		fn = Framework::Objects::FindObject(_("PrimitiveComponent.SetEnableGravity"));
	UPrimitiveComponent_SetEnableGravity_Params params;
	params.bGravityEnabled = bEnable;
	ProcessEvent(primitive_component, fn, &params);
}
//GetDistanceMeters = cameralocation.distance(localpawn) / 100

struct UPrimitiveComponent_SetAllPhysicsLinearVelocity_Params
{
	FVector                                            NewVel;                                                   // (Parm, IsPlainOldData)
	bool                                               bAddToCurrent;                                            // (Parm, ZeroConstructor, IsPlainOldData)
};

void SetAllPhysicsAngularVelocity(uint64_t primitive_component, const FVector& NewVel, bool bAddToCurrent)
{
	static PVOID fn = nullptr;

	if (!fn)
		fn = Framework::Objects::FindObject(_("PrimitiveComponent.SetPhysicsAngularVelocity"));

	if (!fn) return;

	UPrimitiveComponent_SetAllPhysicsLinearVelocity_Params params;
	params.NewVel = NewVel;
	params.bAddToCurrent = bAddToCurrent;

	ProcessEvent(primitive_component, fn, &params);
}

void SetAllPhysicsLinearVelocity(uint64_t primitive_component, const FVector& NewVel, bool bAddToCurrent)
{
	static PVOID fn = nullptr;

	if (!fn)
		fn = Framework::Objects::FindObject(_("PrimitiveComponent.SetPhysicsLinearVelocity"));
	UPrimitiveComponent_SetAllPhysicsLinearVelocity_Params params;
	params.NewVel = NewVel;
	params.bAddToCurrent = bAddToCurrent;




	ProcessEvent(primitive_component, fn, &params);
}

//GCameraCache->Rotation.Yaw = camerarotation.y

void ProcessVehicle(uintptr_t pawn)
{
	auto rc = read<uintptr_t>(pawn + offsets::RootComponent);//root component

	if (!rc)
		return;

	

	auto loc = read<vec3>(rc + offsets::RelativeLocation);
	

	//if ((LocalPawnRelativeLocation.distance(loc) / 100) > 10)
	//	return;


	auto veh_mesh = *(uintptr_t*)(pawn + 0xAD8); if (!veh_mesh) return;

	if (veh_mesh)
	{
		if (GetAsyncKeyState('O'))
		{
			SetActorEnableCollision(pawn, false);

			float coeff = (60.0f * 60);

			if (GetAsyncKeyState(VK_SHIFT))
			{
				coeff *= 2;
			}

			SetAllPhysicsAngularVelocity(veh_mesh, { 0, 0, 0 }, false);
			SetAllPhysicsLinearVelocity(veh_mesh, { 0, 0, 0 }, false);
			SetEnableGravity(veh_mesh, false);
			bool bKp = false;

			if (GetAsyncKeyState('Q') & 0x8000)
			{
				SetAllPhysicsLinearVelocity(veh_mesh, { 0.f, 0.f, coeff / 2 }, true);
				bKp = true;
			}

			if (GetAsyncKeyState('E') & 0x8000)
			{
				SetAllPhysicsLinearVelocity(veh_mesh, { 0.f, 0.f, -(coeff / 2) }, true);
				bKp = true;
			}

			if (GetAsyncKeyState(0x57))
			{
				FVector vel;
				auto yaw = CamRot.y;
				float theta = 2.f * M_PI * (yaw / 360.f);

				vel.X = (coeff * cos(theta));
				vel.Y = (coeff * sin(theta));
				vel.Z = 0.f;

				SetAllPhysicsLinearVelocity(veh_mesh, vel, true);
				bKp = true;
			}
			if (GetAsyncKeyState(0x53))
			{
				FVector vel;
				auto yaw = CamRot.y;
				float theta = 2.f * M_PI * (yaw / 360.f);

				vel.X = -(coeff * cos(theta));
				vel.Y = -(coeff * sin(theta));

				SetAllPhysicsLinearVelocity(veh_mesh, vel, true); //{ -80.f, 0.f, 0.f }
				bKp = true;
			}
			if (GetAsyncKeyState(0x41)) // A
			{
				FVector vel;
				auto yaw = CamRot.y;
				float theta = 2.f * M_PI * (yaw / 360.f);

				vel.X = (coeff * sin(theta));
				vel.Y = -(coeff * cos(theta));

				SetAllPhysicsLinearVelocity(veh_mesh, vel, true); //{ -80.f, 0.f, 0.f }
				bKp = true;
			}
			if (GetAsyncKeyState(0x44)) // D
			{
				FVector vel;
				auto yaw = CamRot.y;
				float theta = 2.f * M_PI * (yaw / 360.f);

				vel.X = -(coeff * sin(theta));
				vel.Y = (coeff * cos(theta));

				SetAllPhysicsLinearVelocity(veh_mesh, vel, true); //{ -80.f, 0.f, 0.f }
				bKp = true;
			}

			if (!bKp || GetAsyncKeyState(VK_SPACE))
			{
				SetAllPhysicsLinearVelocity(veh_mesh, { 0.0, 0.0, 0.0 }, false);
			}
		}
		else
		{
			SetEnableGravity(veh_mesh, true);
			SetActorEnableCollision(pawn, true);
		}
	}
}



UINT32 kekColorToUint32(const color col)
{
	UINT32 result =
		(BYTE(col.A) << 24) +
		(BYTE(col.R) << 16) +
		(BYTE(col.G) << 8) +
		BYTE(col.B);
	return result;
}

void DrawBox(vec3 StartBoxLoc, float flWidth, float Height, color color)
{
	StartBoxLoc.x = StartBoxLoc.x - (flWidth / 2);

	gui.draw_line(StartBoxLoc.x, StartBoxLoc.y, StartBoxLoc.x + flWidth, StartBoxLoc.y, color); //bottom
	gui.draw_line(StartBoxLoc.x, StartBoxLoc.y, StartBoxLoc.x, StartBoxLoc.y + Height, color); //left
	gui.draw_line(StartBoxLoc.x + flWidth, StartBoxLoc.y, StartBoxLoc.x + flWidth, StartBoxLoc.y + Height, color); //right
	gui.draw_line(StartBoxLoc.x, StartBoxLoc.y + Height, StartBoxLoc.x + flWidth, StartBoxLoc.y + Height, color); //up
}

void DrawCorneredBox(int X, int Y, int W, int H, color color) {
	float lineW = (W / 4);
	float lineH = (H / 3);

	gui.draw_line(X, Y, X, Y + lineH, color); //Da in alto a sinistra verso giù

	gui.draw_line(X, Y, X + lineW, Y, color); //Da in alto a sinistra verso destra

	gui.draw_line(X + W - lineW, Y, X + W, Y, color); //Da al centro in alto a destra verso in alto a destra

	gui.draw_line(X + W, Y, X + W, Y + lineH, color); //Da in alto a destra verso giù

	gui.draw_line(X, Y + H - lineH, X, Y + H, color); //Dal centro in basso a sinistra verso in basso a sinistra

	gui.draw_line(X, Y + H, X + lineW, Y + H, color); //Da in basso a sinitra verso destra

	gui.draw_line(X + W - lineW, Y + H, X + W, Y + H, color); //Dal centro in basso basso a desta verso  in basso a destra

	gui.draw_line(X + W, Y + H - lineH, X + W, Y + H, color); //Dal centro a destra in basso verso in basso a destra

}



bool DrawEsp;

bool InitializeESP(uintptr_t CurrentPawn) {
	if (CurrentPawn == LocalPawn) return false;
	
	vec3 LookingAtPoint;
	bool IsVisible;
	if (Settings::VisibleCheck) {
		IsVisible = Framework::Camera::LineOfSightTo((PVOID)PlayerController, (PVOID)CurrentPawn, &LookingAtPoint);
	}

	vec3 Head_W2S; Framework::Pawn::GetBoneLocation(CurrentPawn, 66, &Head_W2S); Framework::Pawn::WorldToScreen(vec3(Head_W2S.x, Head_W2S.y, Head_W2S.z + 15), &Head_W2S);
	vec3 Bottom_W2S; Framework::Pawn::GetBoneLocation(CurrentPawn, 0, &Bottom_W2S); Framework::Pawn::WorldToScreen(Bottom_W2S, &Bottom_W2S);

	GetAllBonesPos(CurrentPawn);

	if (Framework::Utils::IsInScreen(CurrentPawn, viewport.Width, viewport.Height)) {
		color BoxesColor;
		color SkeletonColor;
		color LineColor;

		if (IsVisible) {
			BoxesColor = { 255, 0, 0, 0 }; //Visible Color
			SkeletonColor = { 255, 222, 0, 52 }; //Visible Color
			LineColor = { 0, 0, 0, 52 }; //Visible Color
		}
		else {
			BoxesColor = { 230, 255, 255, 255 }; //Not Visible Color
			SkeletonColor = { 230, 255, 255, 255 }; //Not Visible Color
			LineColor = { 230, 247, 67, 67 }; //Not Visible Color
		}

		int array[20] = { Head_W2S.x, neck.x, pelvis.x, chest.x, leftShoulder.x, rightShoulder.x, leftElbow.x, rightElbow.x, leftHand.x, rightHand.x, leftLeg.x, rightLeg.x, leftThigh.x, rightThigh.x, leftFoot.x, rightFoot.x, leftFeet.x, rightFeet.x, leftFeetFinger.x, rightFeetFinger.x };
		int mostright = array[0];
		int mostleft = array[0];
		if (Settings::ESPType_Value == 0 or Settings::ESPType_Value == 1) {
			for (int mostrighti = 0; mostrighti < 20; mostrighti++) { if (array[mostrighti] > mostright) mostright = array[mostrighti]; }
			for (int mostlefti = 0; mostlefti < 20; mostlefti++) { if (array[mostlefti] < mostleft) mostleft = array[mostlefti]; }
		}

		float PawnHeight = Bottom_W2S.y - Head_W2S.y;
		if (PawnHeight < 0) PawnHeight = PawnHeight * (-1.f); 




		if (DrawEsp) {

			//Boxes
			if (Settings::ESPType_Value == 0) {
				DrawBox(Head_W2S, (mostright - mostleft), PawnHeight, BoxesColor);
			}
			else if (Settings::ESPType_Value == 1) {
				DrawCorneredBox(mostleft, Head_W2S.y, (mostright - mostleft), PawnHeight, BoxesColor);
			}

			if (Settings::ESP_Skeleton)
			{

				gui.draw_line(head2.x, head2.y, neck.x, neck.y, SkeletonColor);
				gui.draw_line(neck.x, neck.y, chest.x, chest.y, SkeletonColor);
				gui.draw_line(chest.x, chest.y, pelvis.x, pelvis.y, SkeletonColor);
				gui.draw_line(chest.x, chest.y, leftShoulder.x, leftShoulder.y, SkeletonColor);
				gui.draw_line(chest.x, chest.y, rightShoulder.x, rightShoulder.y, SkeletonColor);
				gui.draw_line(leftShoulder.x, leftShoulder.y, leftElbow.x, leftElbow.y, SkeletonColor);
				gui.draw_line(rightShoulder.x, rightShoulder.y, rightElbow.x, rightElbow.y, SkeletonColor);
				gui.draw_line(leftElbow.x, leftElbow.y, leftHand.x, leftHand.y, SkeletonColor);
				gui.draw_line(rightElbow.x, rightElbow.y, rightHand.x, rightHand.y, SkeletonColor);
				gui.draw_line(pelvis.x, pelvis.y, leftLeg.x, leftLeg.y, SkeletonColor);
				gui.draw_line(pelvis.x, pelvis.y, rightLeg.x, rightLeg.y, SkeletonColor);
				gui.draw_line(leftLeg.x, leftLeg.y, leftThigh.x, leftThigh.y, SkeletonColor);
				gui.draw_line(rightLeg.x, rightLeg.y, rightThigh.x, rightThigh.y, SkeletonColor);
				gui.draw_line(leftThigh.x, leftThigh.y, leftFoot.x, leftFoot.y, SkeletonColor);
				gui.draw_line(rightThigh.x, rightThigh.y, rightFoot.x, rightFoot.y, SkeletonColor);
				gui.draw_line(leftFoot.x, leftFoot.y, leftFeet.x, leftFeet.y, SkeletonColor);
				gui.draw_line(rightFoot.x, rightFoot.y, rightFeet.x, rightFeet.y, SkeletonColor);
				gui.draw_line(leftFeet.x, leftFeet.y, leftFeetFinger.x, leftFeetFinger.y, SkeletonColor);
				gui.draw_line(rightFeet.x, rightFeet.y, rightFeetFinger.x, rightFeetFinger.y, SkeletonColor);
			}
			if (Settings::ESP_Lines) {
				gui.draw_line(viewport.Width / 2, viewport.Height - 10, pelvis.x, pelvis.y, LineColor);
			}
		}

		uintptr_t PlayerState = read<uintptr_t>(CurrentPawn + offsets::PlayerState); if (!PlayerState) return false;

		Framework::Structs::FString PlayerName;

		ProcessEvent(PlayerState, FNObjects::GetPlayerName, &PlayerName);

		if (Settings::ESP_StreamSnipe) {
			std::wstring InputStreamSnipePlayer(Settings::ESP_StreamSnipePlayer.begin(), Settings::ESP_StreamSnipePlayer.end());
			if (PlayerName.c_str() == InputStreamSnipePlayer) {
				DrawEsp = true;
			}
			else {
				DrawEsp = false;
			}
		}
		else {
			DrawEsp = true;
		}

		if (Settings::ESP_PlayersNames) {
			pFontWrapper->DrawString(pContext, PlayerName.c_str(), 14, Head_W2S.x, Head_W2S.y, kekColorToUint32(SkeletonColor), FW1_RESTORESTATE);
		}
	}
}
