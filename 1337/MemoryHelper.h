#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>

namespace MemoryHelper
{
	uintptr_t PatternScanW(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex = 0);
	uintptr_t PatternScan(const char* sSignature, size_t nSelectResultIndex = 0);
}