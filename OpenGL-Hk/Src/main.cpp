#include <thread>
#include <chrono>
#include <Windows.h>
#include "Hook.h"

void __stdcall MainThread(HINSTANCE instance)
{
	if (Hook::init())
		goto _shutdown;

	while (!GetAsyncKeyState(VK_END))
	{
		Sleep(25);
	}

_shutdown:
	Hook::shutdown();
	FreeLibrary(instance);
}

BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

		// Thread oluþturulmasý
		HANDLE hThread = CreateThread(
			nullptr,
			0,
			(LPTHREAD_START_ROUTINE)MainThread,
			instance,
			0,
			nullptr);

		if (hThread)
		{
			CloseHandle(hThread);
		}
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		// Cleanup
		FreeConsole();
		fclose(stdout);
	}

	return TRUE;
}
