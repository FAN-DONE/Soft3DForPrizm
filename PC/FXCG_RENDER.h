#ifndef FXCG_RENDER_H
#define FXCG_RENDER_H

#include "FXCG_MATH.h"


#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216

#define TRANSFORM_MODE_ROTATION 0x01
#define TRANSFORM_MODE_TRANSLATION 0x02
#define TRANSFORM_MODE_SCALING 0x04

unsigned short* VRAM_base;
fix* ZBUF;

static inline void DrawPoint(unsigned int x, unsigned int y, unsigned short color);
static inline void DrawPoint_s(unsigned int x, unsigned int y, unsigned short color);
static inline unsigned short RGB_to_565(unsigned short r, unsigned short g, unsigned short b);
static inline void VRAMClear(unsigned short color);
static inline void DrawLineVec(Vec2i p0, Vec2i p1, unsigned short color);
static inline void DrawLine(int x0, int y0, int x1, int y1, unsigned short color);
static inline void DrawTriangleVec2i(Vec2i t0, Vec2i t1, Vec2i t2, unsigned short color);
static inline void DrawTriangleVec2iFAST(Vec2i t0, Vec2i t1, Vec2i t2, unsigned short color);
static inline void DrawTriangleVec3iZBUF(Vec3i t0, Vec3i t1, Vec3i t2, unsigned short color);
static inline void DrawTriangleVecLight(Vec3i t0, Vec3fix nor0, Vec3i t1, Vec3fix nor1, Vec3i t2, Vec3fix nor2, Vec3fix lightpos, Vec3fix objectcolor, Vec3fix lightcolor);
static inline Vec2i VertexShaderTriangle(Vec3fix t0, Vec3fix t1, Vec3fix t2, unsigned int transformMode, Vec3fix translation = { 0,0,0 }, Vec3fix scaling = { 0,0,0 }, Vec3fix rotation = { 0,0,0 }, fix angle = 0);

static inline void DrawPoint(unsigned int x, unsigned int y, unsigned short color) {
	unsigned short* VRAM = VRAM_base;
	VRAM += (y * LCD_WIDTH_PX) + x;
	*VRAM = color;
}

static inline void DrawPoint_s(unsigned int x, unsigned int y, unsigned short color) {
	unsigned short* VRAM = VRAM_base;
	if (x > LCD_WIDTH_PX || y > LCD_HEIGHT_PX)
		return;
	VRAM += (y * LCD_WIDTH_PX) + x;
	*VRAM = color;
}

static inline unsigned short RGB_to_565(unsigned short r, unsigned short g, unsigned short b) {
	return (unsigned short)((r << 8) & 0xF800) | ((g << 3) & 0x7E0) | ((b >> 3));
}

static inline void VRAMClear(unsigned short color) {
	unsigned int* VRAM32 = (unsigned int*)VRAM_base;
	unsigned colorr = (unsigned)((color << 16) | color);
	for (unsigned int i = 0; i <= LCD_WIDTH_PX * LCD_HEIGHT_PX / 2; i++)
		*(VRAM32++) = colorr;
}

static inline void DrawLineVec(Vec2i p0, Vec2i p1, unsigned short color) {
	int steep = 0;
	if (abs(p0.x - p1.x) < abs(p0.y - p1.y)) {
		Swapi(p0.x, p0.y);
		Swapi(p1.x, p1.y);
		steep = 1;
	}
	if (p0.x > p1.x) {
		Swapi(p0.x, p1.x);
		Swapi(p0.y, p1.y);
	}
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int derror2 = ((abs(dy)) << 1);
	int error2 = 0;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		if (steep) {
			DrawPoint_s(y, x, color);
		}
		else {
			DrawPoint_s(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (p1.y > p0.y ? 1 : -1);
			error2 -= (dx << 1);
		}
	}
}

static inline void DrawLine(int x0, int y0, int x1, int y1, unsigned short color) {
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1)) {
		Swapi(x0, y0);
		Swapi(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		Swapi(x0, x1);
		Swapi(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = ((abs(dy)) << 1);
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			DrawPoint_s(y, x, color);
		}
		else {
			DrawPoint_s(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= (dx << 1);
		}
	}
}

static inline void DrawTriangleVec2i(Vec2i t0, Vec2i t1, Vec2i t2, unsigned short color) {
	if (t0.y == t1.y && t0.y == t2.y) return;
	if (t0.y > t1.y) SwapVec2i(t0, t1);
	if (t0.y > t2.y) SwapVec2i(t0, t2);
	if (t1.y > t2.y) SwapVec2i(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		int second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		int A = t0.x + (t2.x - t0.x) * alpha;
		int B = second_half ? t1.x + (t2.x - t1.x) * beta : t0.x + (t1.x - t0.x) * beta;
		if (A > B) Swapi(A, B);
		for (int j = A; j <= B; j++) {
			DrawPoint_s(j, t0.y + i, color);
		}
	}
}

static inline void DrawTriangleVec2iFAST(Vec2i t0, Vec2i t1, Vec2i t2, unsigned short color) {
	if (t0.y == t1.y && t0.y == t2.y) return;
	if (t0.y > t1.y) SwapVec2i(t0, t1);
	if (t0.y > t2.y) SwapVec2i(t0, t2);
	if (t1.y > t2.y) SwapVec2i(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		int second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		fix alpha = Divfix(FIX(i), FIX(total_height));
		fix beta = Divfix(FIX(i - (second_half ? t1.y - t0.y : 0)), FIX(segment_height));
		int A = UNFIX(FIX(t0.x) + Mulfix(FIX(t2.x - t0.x), alpha));
		int B = second_half ? UNFIX(FIX(t1.x) + Mulfix(FIX(t2.x - t1.x), beta)) : UNFIX(FIX(t0.x) + Mulfix(FIX(t1.x - t0.x), beta));
		if (A > B) Swapi(A, B);
		for (int j = A; j <= B; j++) {
			DrawPoint_s(j, t0.y + i, color);
		}
	}
}

static inline void DrawTriangleVec3iZBUF(Vec3i t0, Vec3i t1, Vec3i t2, unsigned short color) {
	if (t0.y == t1.y && t0.y == t2.y) return;
	int t0x = t0.x, t1x = t1.x, t2x = t2.x;
	int t0y = t0.y, t1y = t1.y, t2y = t2.y;
	if (t0y > t1y) Swapi(t0y, t1y);
	if (t0y > t2y) Swapi(t0y, t2y);
	if (t1y > t2y) Swapi(t1y, t2y);
	if (t0x > t1x) Swapi(t0x, t1x);
	if (t0x > t2x) Swapi(t0x, t2x);
	if (t1x > t2x) Swapi(t1x, t2x);
	for (int i = t0x; i < t2x; i++) {
		for (int j = t0y; j < t2y; j++) {
			Vec3i P = { i,j,0 };
			Vec3fix bc_screen = Barycentric(t0, t1, t2, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z += Mulfix(t0.z, bc_screen.x);
			P.z += Mulfix(t1.z, bc_screen.y);
			P.z += Mulfix(t2.z, bc_screen.z);
			if (P.z < (ZBUF[j + i * LCD_WIDTH_PX])) {
				DrawPoint_s(i, j, color);
				ZBUF[j + i * LCD_WIDTH_PX] = P.z;
			}
		}
	}
}

static inline void DrawTriangleVecLight(Vec3i t0, Vec3fix nor0, Vec3i t1, Vec3fix nor1, Vec3i t2, Vec3fix nor2, Vec3fix lightpos, Vec3fix objectcolor, Vec3fix lightcolor) {
	if (t0.y == t1.y && t0.y == t2.y) return;
	int t0x = t0.x, t1x = t1.x, t2x = t2.x;
	int t0y = t0.y, t1y = t1.y, t2y = t2.y;
	if (t0y > t1y) Swapi(t0y, t1y);
	if (t0y > t2y) Swapi(t0y, t2y);
	if (t1y > t2y) Swapi(t1y, t2y);
	if (t0x > t1x) Swapi(t0x, t1x);
	if (t0x > t2x) Swapi(t0x, t2x);
	if (t1x > t2x) Swapi(t1x, t2x);
	for (int i = t0x; i < t2x; i++) {
		for (int j = t0y; j < t2y; j++) {
			Vec3i P = { i,j,0 };
			Vec3fix bc_screen = Barycentric(t0, t1, t2, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z += Mulfix(t0.z, bc_screen.x);
			P.z += Mulfix(t1.z, bc_screen.y);
			P.z += Mulfix(t2.z, bc_screen.z);
			if (P.z < (ZBUF[j + i * LCD_WIDTH_PX])) {
				Vec3fix Pnor;
				Pnor.x = Mulfix(bc_screen.x, nor0.x + nor1.x + nor2.x);
				Pnor.y = Mulfix(bc_screen.y, nor0.y + nor1.y + nor2.y);
				Pnor.z = Mulfix(bc_screen.z, nor0.z + nor1.z + nor2.z);
				Vec3fix lightDir;
				lightDir.x = lightpos.x - FIX(P.x);
				lightDir.y = lightpos.y - FIX(P.y);
				lightDir.z = lightpos.z - P.z;
				Normalizefix(&lightDir);
				Normalizefix(&Pnor);
				fix diff = abs(Dotfix(Pnor, lightDir));
				Vec3i diffuse;
				diffuse.x = Mulfix(diff, objectcolor.x) * lightcolor.x >> 15;
				diffuse.x = diffuse.x * 255 >> 15;
				diffuse.y = Mulfix(diff, objectcolor.y) * lightcolor.y >> 15;
				diffuse.y = diffuse.y * 255 >> 15;
				diffuse.z = Mulfix(diff, objectcolor.z) * lightcolor.z >> 15;
				diffuse.z = diffuse.z * 255 >> 15;
				unsigned short endcolor = RGB_to_565(diffuse.x, diffuse.y, diffuse.z);
				DrawPoint_s(i, j, endcolor);
				ZBUF[j + i * LCD_WIDTH_PX] = P.z;
			}
		}
	}
}

static inline Vec2i VertexShaderTriangle(Vec3fix t0, Vec3fix t1, Vec3fix t2, unsigned int transformMode, Vec3fix translation, Vec3fix scaling, Vec3fix rotation, fix angle) {
	if ((transformMode & TRANSFORM_MODE_TRANSLATION) == TRANSFORM_MODE_TRANSLATION) {

	}
}

#endif