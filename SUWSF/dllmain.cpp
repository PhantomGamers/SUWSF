// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <Psapi.h>
#include "../external/IniReader/IniReader.h"
#include <algorithm>
#include "AspectRatio.h"
#include "dllmain.h"
#include "Logging.h"
#include "UserSettings.h"
#include "GenericPatch.h"

void Init()
{
	SetThreadLocale(LOCALE_INVARIANT);
	ClearLogFiles();
	DBOUT(APPNAME << " VERSION " << VERSION << " INITIALIZED.");

	GetModuleFileName(nullptr, szFileName, MAX_PATH);
	mInfo = GetModuleInfo(szFileName);

	UserSettings::Init();
	AspectRatio::Init();
	GenericPatch::Init();
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)Init, nullptr, NULL, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
