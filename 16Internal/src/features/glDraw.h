#pragma once
#pragma comment (lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include <cmath>
#include "../utils/vec.hpp"

class GL
{
public:
	GLint viewport[4];
	const GLubyte red[3] = { 255, 0, 0 };
	const GLubyte green[3] = { 0, 255, 0 };
	const GLubyte blue[3] = { 0, 0, 255 };

	void SetupOrtho();
	void Restore();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);
	void DrawCrosshair(vec2& l1p1, vec2& l1p2, vec2& l2p1, vec2& l2p2, float lineWidth, const GLubyte color[3]);
	void DrawESPBox(vec2 head, vec2 feet, float lineWidth, const GLubyte color[3]);

	bool WorldToScreen(vec3 pos, vec2& screen, float matrix[16], int windowWidth, int windowHeight);
};