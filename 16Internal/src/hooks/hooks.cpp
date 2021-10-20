#include "hooks.h"

namespace Hooks
{
	using namespace std;
	//=========================================================================//
	uintptr_t clientModule = (uintptr_t)GetModuleHandle(L"client.dll");
	uintptr_t hwModule = (uintptr_t)GetModuleHandle(L"hw.dll");
	//=========================================================================//
	uintptr_t wglSwapBuffersAddress = (uintptr_t)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	uintptr_t HUD_AddEntityAddress = *(uintptr_t*)(hwModule + 0x122EDB0);
	uintptr_t HUD_ProcessPlayerStateAddress = *(uintptr_t*)(hwModule + 0x122EDD0);

	f_wglSwapBuffers owglSwapBuffers;
	f_HUD_AddEntity oHUD_AddEntity;
	f_HUD_ProcessPlayerState oHUD_ProcessPlayerState;
	//=========================================================================//
	GL gl;

	vector<pair<vec3, vec3>> ent3DPositions;
	//=========================================================================//

	void Init()
	{
		owglSwapBuffers = (f_wglSwapBuffers)DetourFunction((PBYTE)wglSwapBuffersAddress, (PBYTE)hkwglSwapBuffers);
		oHUD_AddEntity = (f_HUD_AddEntity)DetourFunction((PBYTE)HUD_AddEntityAddress, (PBYTE)hkHUD_AddEntity);
		oHUD_ProcessPlayerState = (f_HUD_ProcessPlayerState)DetourFunction((PBYTE)HUD_ProcessPlayerStateAddress, (PBYTE)hkHUD_ProcessPlayerState);
	}

	BOOL _stdcall hkwglSwapBuffers(HDC hdc)
	{
		gl.SetupOrtho();

		// ESP
		float viewMatrix[16];
		memcpy(&viewMatrix, (PBYTE*)(hwModule + 0xEC9780), sizeof(viewMatrix));

		for (const pair<vec3, vec3>& ent : ent3DPositions)
		{
			vec2 entPosHead2D, entPosFeet2D;
			if (gl.WorldToScreen(ent.first, entPosHead2D, viewMatrix, gl.viewport[2], gl.viewport[3]) && gl.WorldToScreen(ent.second, entPosFeet2D, viewMatrix, gl.viewport[2], gl.viewport[3]))
			{
				gl.DrawESPBox(entPosHead2D, entPosFeet2D, 1, gl.red);
			}
		}
		ent3DPositions.clear();

		// Crosshair
		float punchX = *(float*)(hwModule + 0x122E324);
		float punchY = *(float*)(hwModule + 0x122E324+4);
		float SWidth = (float)(gl.viewport[2] / 2);
		float SHeight = (float)(gl.viewport[3] / 2);
		float SWidth2 = (float)(gl.viewport[2] / 90);
		float SHeight2 = (float)(gl.viewport[3] / 90);
		SWidth -= (SWidth2 * (punchY));
		SHeight += (SHeight2 * (punchX));
		vec2 l1p1, l1p2, l2p1, l2p2;
		l1p1.x = SWidth - 3.0f; l1p1.y = SHeight + 1.0f;
		l1p2.x = SWidth + 4.0f; l1p2.y = SHeight + 1.0f;
		l2p1.x = SWidth + 1.0f; l2p1.y = SHeight - 3.0f;
		l2p2.x = SWidth + 1.0f; l2p2.y = SHeight + 4.0f;
		gl.DrawCrosshair(l1p1, l1p2, l2p1, l2p2, 1, gl.green);
		//

		gl.Restore();
		return owglSwapBuffers(hdc);
	}

	int __cdecl hkHUD_AddEntity(int type, struct cl_entity_s* ent, const char* modelname)
	{
		if (ent->player)
		{
			// Get ent team
			char team = *(char*)(clientModule + 0x12F44C + ent->index * 0x74);
			int teamId;
			if (team == 'C')
				teamId = 2;
			else if (team == 'T')
				teamId = 1;
			// Local player team
			uintptr_t localPlayerTeam = *(uintptr_t*)(clientModule + 0x100DF4);
			if (localPlayerTeam != teamId)
			{
				vec3 entPosHead = ent->origin;	entPosHead.z += 28; //entPosHead.x += 5;
				vec3 entPosFeet = ent->origin;	entPosFeet.z -= 35; //entPosFeet.x += 5;
				ent3DPositions.push_back(make_pair(entPosHead, entPosFeet));
			}
		}
		return oHUD_AddEntity(type, ent, modelname);
	}

	void __cdecl hkHUD_ProcessPlayerState(struct entity_state_s* dst, const struct entity_state_s* src)
	{

		return oHUD_ProcessPlayerState(dst, src);
	}

	void Unhook()
	{
		DetourRemove((PBYTE)owglSwapBuffers, (PBYTE)hkwglSwapBuffers);
		DetourRemove((PBYTE)oHUD_AddEntity, (PBYTE)hkHUD_AddEntity);
		DetourRemove((PBYTE)oHUD_ProcessPlayerState, (PBYTE)hkHUD_ProcessPlayerState);
	}
}