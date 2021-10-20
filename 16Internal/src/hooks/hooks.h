#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

#include "../SDK/common/cl_entity.h"

#include "../features/glDraw.h"
#include "../deps/detours.h"

namespace Hooks
{
	//=====================================================================================================================//
	typedef BOOL(__stdcall* f_wglSwapBuffers)(HDC hdc);
	BOOL _stdcall hkwglSwapBuffers(HDC hdc);

	typedef int(__cdecl* f_HUD_AddEntity)(int type, struct cl_entity_s* ent, const char* modelname);
	int __cdecl hkHUD_AddEntity(int type, struct cl_entity_s* ent, const char* modelname);

	typedef void(__cdecl* f_HUD_ProcessPlayerState)(struct entity_state_s* dst, const struct entity_state_s* src);
	void __cdecl hkHUD_ProcessPlayerState(struct entity_state_s* dst, const struct entity_state_s* src);
	//=====================================================================================================================//

	void Init();
	void Unhook();
}