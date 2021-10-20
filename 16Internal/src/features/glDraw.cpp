#include "glDraw.h"

void GL::SetupOrtho()
{
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glPushMatrix();
	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glViewport(0, 0, viewport[2], viewport[3]);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, viewport[2], viewport[3], 0, 0, 1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glDisable(GL_DEPTH_TEST);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, viewport[2], viewport[3], 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void GL::Restore()
{
	glPopMatrix();
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);
}

void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
	// Immediate mode
	glColor4ub(color[0], color[1], color[2], 255);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}
void GL::DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor4ub(color[0], color[1], color[2], 255);
	glVertex2f(x - 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y - 0.5f);
	glEnd();
}
void GL::DrawCrosshair(vec2& l1p1, vec2& l1p2, vec2& l2p1, vec2& l2p2, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glColor4ub(color[0], color[1], color[2], 255);
	glVertex2f(l1p1.x, l1p1.y);
	glVertex2f(l1p2.x, l1p2.y);
	glVertex2f(l2p1.x, l2p1.y);
	glVertex2f(l2p2.x, l2p2.y);
	glEnd();
}
void GL::DrawESPBox(vec2 head, vec2 feet, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);

	int height = abs(head.y - feet.y);
	vec2 tl, tr, bl, br;

	tl.x = head.x - height / 4;
	tr.x = head.x + height / 4;
	tl.y = tr.y = head.y;

	bl.x = feet.x - height / 4;
	br.x = feet.x + height / 4;
	bl.y = br.y = feet.y;

	// outline
	glColor4ub(0, 0, 0, 255/2);
	glVertex2f(tl.x - 1, tl.y - 1);
	glVertex2f(tr.x + 1, tr.y - 1);
	glVertex2f(br.x + 1, br.y + 1);
	glVertex2f(bl.x - 1, bl.y + 1);
	glVertex2f(tl.x - 1, tl.y);
	// inline
	glVertex2f(tl.x + 1, tl.y + 1);
	glVertex2f(tr.x - 1, tr.y + 1);
	glVertex2f(br.x - 1, br.y - 1);
	glVertex2f(bl.x + 1, bl.y - 1);
	glVertex2f(tl.x + 1, tl.y + 1);

	glColor4ub(color[0], color[1], color[2], 255);
	glVertex2f(tl.x, tl.y);
	glVertex2f(tr.x, tr.y);
	glVertex2f(br.x, br.y);
	glVertex2f(bl.x, bl.y);
	glVertex2f(tl.x, tl.y);


	glEnd();
}

bool GL::WorldToScreen(vec3 pos, vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}