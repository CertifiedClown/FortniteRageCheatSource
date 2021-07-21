#pragma once

bool IsValidPointer(uintptr_t address) {
	if (!IsBadWritePtr((LPVOID)address, (UINT_PTR)8)) return TRUE;
	else return false;
}

template<typename ReadT>
ReadT read(uintptr_t address, const ReadT& def = ReadT()) {
	if (IsValidPointer(address)) {
		return *(ReadT*)address;
	}
}

template<typename WriteT>
WriteT write(uintptr_t address, WriteT value, const WriteT& def = WriteT()) {
	if (IsValidPointer(address)) {
		*(WriteT*)((PBYTE)address) = value;
	}
	return 1;
}

class UClass {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

class UObject {
public:
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE _padding_0[0x8];
	UObject* Outer;
	/*
	inline void ProcessEvent(void* fn, void* parms)
	{
		auto vtable = *reinterpret_cast<void***>(this);
		auto processEventFn = static_cast<void(*)(void*, void*, void*)>(vtable[0x44]);
		processEventFn(this, fn, parms);
	}
	*/
	inline BOOLEAN IsA(PVOID parentClass) {
		for (auto super = this->Class; super; super = super->SuperClass) {
			if (super == parentClass) {
				return TRUE;
			}
		}

		return FALSE;
	}
};

class FUObjectItem {
public:
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

class TUObjectArray {
public:
	FUObjectItem* Objects[9];
};

class GObjects {
public:
	TUObjectArray* ObjectArray;
	BYTE _padding_0[0xC];
	DWORD ObjectCount;
};

GObjects* objects = nullptr;
bool once = false;
namespace Framework {
	namespace Structs {


		struct FMatrix
		{
			float M[4][4];
		};
		static FMatrix* myMatrix = new FMatrix();


		template<class T>
		struct TArray
		{
			friend struct FString;

		public:
			inline TArray()
			{
				Data = nullptr;
				Count = Max = 0;
			};

			inline int Num() const
			{
				return Count;
			};

			inline T& operator[](int i)
			{
				return Data[i];
			};

			inline const T& operator[](int i) const
			{
				return Data[i];
			};

			inline bool IsValidIndex(int i) const
			{
				return i < Num();
			}

		private:
			T* Data;
			int32_t Count;
			int32_t Max;
		};

		struct FString : private TArray<wchar_t>
		{
			inline FString()
			{
			};

			FString(const wchar_t* other)
			{
				Max = Count = *other ? std::wcslen(other) + 1 : 0;

				if (Count)
				{
					Data = const_cast<wchar_t*>(other);
				}
			};

			inline bool IsValid() const
			{
				return Data != nullptr;
			}

			inline const wchar_t* c_str() const
			{
				return Data;
			}

			std::string ToString() const
			{
				auto length = std::wcslen(Data);
				std::string str(length, '\0');
				std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);
				return str;
			}
		};
	}
	namespace Pawn {
		static bool GetBoneLocation(uintptr_t CurrentActor, int id, vec3* out)
		{
			uintptr_t mesh = read<uintptr_t>(CurrentActor + offsets::Mesh); if (!mesh) return false;

			auto fGetBoneMatrix = ((Structs::FMatrix * (__fastcall*)(uintptr_t, Structs::FMatrix*, int))(BoneMatrix));
			SpoofCall(fGetBoneMatrix, mesh, Structs::myMatrix, id);

			out->x = Structs::myMatrix->M[3][0];
			out->y = Structs::myMatrix->M[3][1];
			out->z = Structs::myMatrix->M[3][2];

			return true;
		}

		static bool WorldToScreen(vec3 WorldLocation, vec3* out)
		{
			auto WorldToScreen = reinterpret_cast<bool(__fastcall*)(uintptr_t pPlayerController, vec3 vWorldPos, vec3 * vScreenPosOut, char)>(ProjectWorldToScreen);

			SpoofCall(WorldToScreen, (uintptr_t)PlayerController, WorldLocation, out, (char)0);

			return true;
		}

		static void SetControlRotation(vec3 NewRotation, bool bResetCamera = false)
		{
			auto SetControlRotation_ = (*(void(__fastcall**)(uintptr_t Controller, vec3 NewRotation, bool bResetCamera))(*(uintptr_t*)PlayerController + 0x688));
			SpoofCall(SetControlRotation_, PlayerController, NewRotation, bResetCamera);
		}
	}
	namespace Utils {
		inline float GetDistanceLength(vec3 start, vec3 end) {
			return float(sqrtf(powf(end.x - start.x, 2.0) + powf(end.y - start.y, 2.0) + powf(end.z - start.z, 2.0)));
		}

		bool InFov(uintptr_t CurrentPawn, int FovValue, int Width, int Height) {
			vec3 HeadPos; Pawn::GetBoneLocation(CurrentPawn, 66, &HeadPos); Pawn::WorldToScreen(HeadPos, &HeadPos);
			auto dx = HeadPos.x - (Width / 2);
			auto dy = HeadPos.y - (Height / 2);
			auto dist = sqrtf(dx * dx + dy * dy);
			
			if (dist < FovValue) {
				return true;
			}
			else {
				return false;
			}
		}

		bool IsInScreen(uintptr_t CurrentPawn, int Width, int Height) {
			if (CurrentPawn) {
				vec3 HeadPos; Pawn::GetBoneLocation(CurrentPawn, 66, &HeadPos); Pawn::WorldToScreen(HeadPos, &HeadPos);
				if (((HeadPos.x <= 0 or HeadPos.x > Width) and (HeadPos.y <= 0 or HeadPos.y > Height)) or ((HeadPos.x <= 0 or HeadPos.x > Width) or (HeadPos.y <= 0 or HeadPos.y > Height))) {
					return false;
				}
				else {
					return true;
				}
			}
		}
	}
	namespace Objects {
		
		static void FreeObjectName(__int64 address) {
			auto func = reinterpret_cast<__int64(__fastcall*)(__int64 a1)>(FreeFn);
			SpoofCall(func, address);
		}
		
		static const char* GetUintObjectName(uintptr_t Object)
		{
			if (Object == NULL) return "";

			auto fGetObjName = reinterpret_cast<Structs::FString * (__fastcall*)(int* index, Structs::FString * res)>(GetNameByIndex);

			int index = *(int*)(Object + 0x18);

			Structs::FString result;
			SpoofCall(fGetObjName, &index, &result);

			if (result.c_str() == NULL) return "";

			auto result_str = result.ToString();

			if (result.c_str() != NULL)
				FreeObjectName((__int64)result.c_str());

			return result_str.c_str();
		}


		static const char* GetUObjectName(UObject* Object)
		{
			if (Object == NULL) return "";

			auto fGetObjName = reinterpret_cast<Structs::FString * (__fastcall*)(int* index, Structs::FString * res)>(GetNameByIndex);

			int index = *(int*)(reinterpret_cast<uint64_t>(Object) + 0x18);

			Structs::FString result;
			SpoofCall(fGetObjName, &index, &result);

			if (result.c_str() == NULL) return "";

			auto result_str = result.ToString();

			if (result.c_str() != NULL)
				FreeObjectName((__int64)result.c_str());

			return result_str.c_str();
		}


		static const char* GetUObjectNameLoop(UObject* Object) {
			std::string name("");
			for (auto i = 0; Object; Object = Object->Outer, ++i) {

				auto fGetObjName = reinterpret_cast<Structs::FString * (__fastcall*)(int* index, Structs::FString * res)>(GetNameByIndex);

				int index = *(int*)(reinterpret_cast<uint64_t>(Object) + 0x18);

				Structs::FString fObjectName;
				SpoofCall(fGetObjName, &index, &fObjectName);

				if (!fObjectName.IsValid()) {
					break;
				}

				auto objectName = fObjectName.ToString();


				name = objectName + std::string(i > 0 ? "." : "") + name;
				FreeObjectName((uintptr_t)fObjectName.c_str());
			}

			return name.c_str();
		}

		PVOID FindObject(std::string name) {
			bool once = false;
			if (!once)
			{
				auto UObjectPtr = MemoryHelper::PatternScan(signatures::Gobject_Sig);
				objects = decltype(objects)(RVA(UObjectPtr, 7));
				//std::cout << "obj  " << std::hex << objects << "\n";
				//std::cout << "base  " << std::hex << GetModuleHandleA(0) << "\n";

				once = true;
			}
			for (auto array : objects->ObjectArray->Objects) {
				auto fuObject = array;
				//Sleep(1);
				for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject) {
					//Sleep(1);
					auto object = fuObject->Object;
					if (object->ObjectFlags != 0x41) {}
					else {
						std::cout << "";
						if (strstr(GetUObjectNameLoop(object), name.c_str())) return object;
					}

					//log << GetUObjectName(object) << "\n";
				}
			}
			return 0;
		}

	}
	namespace Camera {
		static float GetFOVAngle(uintptr_t PlayerCameraManager)
		{
			auto GetFOVAngle = reinterpret_cast<float(*)(UINT64, char*)>(*(ULONG_PTR*)(*(ULONG_PTR*)PlayerCameraManager + 0x6D0));
			return SpoofCall(GetFOVAngle, (ULONG_PTR)PlayerCameraManager, (char*)0);
		}

		static vec3 GetCameraLocation(uintptr_t PlayerCameraManager)
		{
			auto GetCameraLocation = reinterpret_cast<vec3(*)(UINT64, char*)>(*(ULONG_PTR*)(*(ULONG_PTR*)PlayerCameraManager + 0x718));
			return SpoofCall(GetCameraLocation, (ULONG_PTR)PlayerCameraManager, (char*)0);
		}

		static vec3 GetCameraRotation(uintptr_t PlayerCameraManager)
		{
			auto GetCameraRotation = reinterpret_cast<vec3(*)(UINT64, char*)>(*(ULONG_PTR*)(*(ULONG_PTR*)PlayerCameraManager + 0x710));
			return SpoofCall(GetCameraRotation, (ULONG_PTR)PlayerCameraManager, (char*)0);
		}

		static bool LineOfSightTo(PVOID PlayerController, PVOID Actor, vec3* ViewPoint) {
			auto LOSTo = reinterpret_cast<bool(__fastcall*)(PVOID PlayerController, PVOID Actor, vec3 * ViewPoint)>(LineOfS);
			return SpoofCall(LOSTo, PlayerController, Actor, ViewPoint);
		}

		static bool GetPlayerViewPoint(uintptr_t PlayerController, vec3* vCameraPos, vec3* vCameraRot)
		{
			if (!PlayerController) return false;

			static uintptr_t pGetPlayerViewPoint = 0;
			if (!pGetPlayerViewPoint)
			{
				uintptr_t VTable = *(uintptr_t*)PlayerController;
				if (!VTable)  return false;

				pGetPlayerViewPoint = *(uintptr_t*)(VTable + 0x708);
				if (!pGetPlayerViewPoint)  return false;
			}

			auto GetPlayerViewPoint = reinterpret_cast<void(__fastcall*)(uintptr_t, vec3*, vec3*)>(pGetPlayerViewPoint);

			SpoofCall(GetPlayerViewPoint, (uintptr_t)PlayerController, vCameraPos, vCameraRot);

			return true;
		}

		vec3 CalculateNewRotation(uintptr_t CurrentPawn, vec3 LocalRelativeLocation, bool prediction) {
			vec3 NewRotationVector = { 0, 0, 0 };

			vec3 Head_No_W2S, Head_W2S;
			Pawn::GetBoneLocation(CurrentPawn, 66, &Head_No_W2S); Pawn::WorldToScreen(vec3(Head_No_W2S.x, Head_No_W2S.y, Head_No_W2S.z + 20), &Head_W2S);

			vec3 PredictionCalculated;

			if (prediction) {
				float distance = Utils::GetDistanceLength(LocalRelativeLocation, Head_W2S) / 250;
				uintptr_t CurrentPawnRootComponent = read<uintptr_t>(CurrentPawn + offsets::RootComponent);
				vec3 CurrentPawnVelocity = read<vec3>(CurrentPawnRootComponent + offsets::ComponentVelocity);

				vec3 PositionRecalculated = Head_No_W2S;
				float BulletGravity = fabs(-504);
				float Time = distance / fabs(30000);
				float BulletDrop = (BulletGravity / 250) * Time * Time;
				PositionRecalculated.z += BulletDrop * 120;
				PositionRecalculated.x += Time * (CurrentPawnVelocity.x);
				PositionRecalculated.y += Time * (CurrentPawnVelocity.y);
				PositionRecalculated.z += Time * (CurrentPawnVelocity.z);
				PredictionCalculated = PositionRecalculated;
			}
			else {
				PredictionCalculated = Head_No_W2S;
			}

			vec3 VectorPos = PredictionCalculated - CamLoc;

			float distance = VectorPos.Length();

			NewRotationVector.x = -((SpoofRuntime::acosf_(VectorPos.z / distance) * (float)(180.0f / M_PI)) - 90.f);
			NewRotationVector.y = SpoofRuntime::atan2f_(VectorPos.y, VectorPos.x) * (float)(180.0f / M_PI);

			return NewRotationVector;
		}
	}
}


namespace No_Discord {

	uintptr_t module = (uintptr_t)LI_FN(GetModuleHandleA)(_("DiscordHook64.dll"));

	std::vector<uintptr_t> pCreatedHooksArray;
	bool CreateHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall)
	{
		static uintptr_t addrCreateHook = NULL;

		if (!addrCreateHook)
			addrCreateHook = MemoryHelper::PatternScanW(module, _("41 57 41 56 56 57 55 53 48 83 EC 68 4D 89 C6 49 89 D7"));

		if (!addrCreateHook)
			return false;

		using CreateHook_t = uint64_t(__fastcall*)(LPVOID, LPVOID, LPVOID*);
		auto fnCreateHook = (CreateHook_t)addrCreateHook;

		return SpoofCall(fnCreateHook, (void*)pOriginal, (void*)pHookedFunction, (void**)pOriginalCall) == 0 ? true : false;
	}

	bool EnableHookQue()
	{
		static uintptr_t addrEnableHookQueu = NULL;

		if (!addrEnableHookQueu)
			addrEnableHookQueu = MemoryHelper::PatternScanW(module, _("41 57 41 56 41 55 41 54 56 57 55 53 48 83 EC 38 48 ? ? ? ? ? ? 48 31 E0 48 89 44 24 30 BE 01 00 00 00 31 C0 F0 ? ? ? ? ? ? ? 74 2B"));

		if (!addrEnableHookQueu)
			return false;

		using EnableHookQueu_t = uint64_t(__stdcall*)(VOID);
		auto fnEnableHookQueu = (EnableHookQueu_t)addrEnableHookQueu;

		return SpoofCall(fnEnableHookQueu) == 0 ? true : false;
	}

	short GetAsyncKeyState(int key)
	{
		static uintptr_t GetAsyncKeyState_addr;
		if (!GetAsyncKeyState_addr)
			GetAsyncKeyState_addr = MemoryHelper::PatternScanW(module, _("48 FF ? ? ? ? ? CC CC CC CC CC CC CC CC CC 48 FF ? ? ? ? ? CC CC CC CC CC CC CC CC CC 48 83 EC 28 48 ? ? ? ? ? ? 48 85 C9"));
		if (!GetAsyncKeyState_addr)
			return false;

		auto queuehook = ((short(__fastcall*)(int))(GetAsyncKeyState_addr));
		return SpoofCall(queuehook, key);
	}

	bool EnableHook(uintptr_t pTarget, bool bIsEnabled)
	{
		static uintptr_t addrEnableHook = NULL;

		if (!addrEnableHook)
			addrEnableHook = MemoryHelper::PatternScanW(module, _("41 56 56 57 53 48 83 EC 28 49 89 CE BF 01 00 00 00 31 C0 F0 ? ? ? ? ? ? ? 74"));

		if (!addrEnableHook)
			return false;

		using EnableHook_t = uint64_t(__fastcall*)(LPVOID, bool);
		auto fnEnableHook = (EnableHook_t)addrEnableHook;

		return SpoofCall(fnEnableHook, (void*)pTarget, bIsEnabled) == 0 ? true : false;
	}


	short SetCursorPos(int x, int y)
	{
		static uintptr_t addrSetCursorPos = NULL;

		if (!addrSetCursorPos)
		{
			addrSetCursorPos = MemoryHelper::PatternScanW(module,
				_("8A 05 ? ? ? ? 84 C0 74 12"));
		}

		if (!addrSetCursorPos)
			return false;

		using SetCursorPos_t = short(__fastcall*)(int, int);
		auto fnSetCursorPos = (SetCursorPos_t)addrSetCursorPos;

		return fnSetCursorPos(x, y);
	}

	bool InsertHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall)
	{
		bool bAlreadyCreated = false;
		for (auto _Hook : pCreatedHooksArray)
		{
			if (_Hook == pOriginal)
			{
				bAlreadyCreated = true;
				break;
			}
		}

		if (!bAlreadyCreated)
			bAlreadyCreated = CreateHook(pOriginal, pHookedFunction, pOriginalCall);

		if (bAlreadyCreated)
			if (EnableHook(pOriginal, true))
				if (EnableHookQue())
					return true;

		return false;
	}
}