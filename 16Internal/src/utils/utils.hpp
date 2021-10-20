#pragma once
#include <windows.h>
#include <iostream>

namespace Utils
{
	FILE* f;
	void AttachConsole()
	{
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
	}
	void DetachConsole()
	{
		FreeConsole();
		fclose(f);
	}
}