// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "external/Blackbone/src/BlackBone/Patterns/PatternSearch.h"
#include <iostream>
#include <Psapi.h>
#include "include/main.h"
#include "external/IniReader/IniReader.h"
#include <algorithm>

using namespace blackbone;

const std::string APPNAME("SUWSF");

void Init()
{
	DBOUT(APPNAME << " INITIALIZED.");

	CIniReader ini(APPNAME + ".ini");

	auto byteString = ini.ReadString("AspectRatio", "TargetBytes", "39 8E E3 3F");
	DBOUT("byteString " << byteString);
	auto resString = ini.ReadString("AspectRatio", "DesiredResolution", "3440x1440");
	DBOUT("resString " << resString);

	auto resStrings = split(resString, 'x');
	auto desiredWidth = std::stoi(resStrings.at(0));
	DBOUT("Width: " << desiredWidth);
	auto desiredHeight = std::stoi(resStrings.at(1));
	DBOUT("Height: " << desiredHeight);

	auto bytesVec = StringToHex(byteString);
	std::string bytes = "";
	for (int i = 0; i < bytesVec.size(); i++)
	{
		bytes += bytesVec[i];
	}

	PatternSearch ps{ bytes };

	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	MODULEINFO mInfo = GetModuleInfo((char*)szFileName);
	auto base = mInfo.lpBaseOfDll;
	auto size = mInfo.SizeOfImage;

	std::vector<ptr_t> results;
	auto matchesFound = ps.Search(base, size, results);
	DBOUT("Found " << matchesFound << " matches");

	for (auto result : results)
	{
		float* addr = reinterpret_cast<float*>(result);

		unsigned long OldProtection;
		VirtualProtect((LPVOID)(result), sizeof(float), PAGE_EXECUTE_READWRITE, &OldProtection);

		*addr = (float)desiredWidth / desiredHeight;

		VirtualProtect((LPVOID)(result), sizeof(float), OldProtection, _Post_ _Notnull_ NULL);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Init, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}