#ifndef FXCG_RENDER_H
#define FXCG_RENDER_H

#include "FXCG_MATH.h"
#include "FXCG_DISPLAY.h"

#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216


fix *ZBUF;//Z缓冲
Point3 __POINT0;
Point3 __POINT1;
Point3 __POINT2;


//===========================================================
//=====================基本图形绘制==========================
//===========================================================

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

static inline void DrawTriangleVec2iFAST(Vec2i* t, unsigned short color) {
	if (t[0].y == t[1].y && t[0].y == t[2].y) return;
	if (t[0].y > t[1].y) SwapVec2i(t[0], t[1]);
	if (t[0].y > t[2].y) SwapVec2i(t[0], t[2]);
	if (t[1].y > t[2].y) SwapVec2i(t[1], t[2]);
	int total_height = t[2].y - t[0].y;
	for (int i = 0; i < total_height; i++) {
		int second_half = i > t[1].y - t[0].y || t[1].y == t[0].y;
		int segment_height = second_half ? t[2].y - t[1].y : t[1].y - t[0].y;
		fix alpha = Divfix(FIX(i), FIX(total_height));
		fix beta = Divfix(FIX(i - (second_half ? t[1].y - t[0].y : 0)), FIX(segment_height));
		int A = UNFIX(FIX(t[0].x) + Mulfix(FIX(t[2].x - t[0].x), alpha));
		int B = second_half ? UNFIX(FIX(t[1].x) + Mulfix(FIX(t[2].x - t[1].x), beta)) : UNFIX(FIX(t[0].x) + Mulfix(FIX(t[1].x - t[0].x), beta));
		if (A > B) Swapi(A, B);
		for (int j = A; j <= B; j++) {
			DrawPoint_s(j, t[0].y + i, color);
		}
	}
}


//===========================================================
//=====================固定渲染管线==========================
//===========================================================

static inline void ShaderInit(Point3 t0, Point3 t1, Point3 t2) {
	__POINT0 = t0;
	__POINT1 = t1;
	__POINT2 = t2;
}

static inline void RotationY(fix angle) {
	Vec3fix temp0 = __POINT0.position;
	Vec3fix temp1 = __POINT1.position;
	Vec3fix temp2 = __POINT2.position;
	fix __SIN = Sinfix(angle);
	fix __COS = Cosfix(angle);
	__POINT0.position.x = Mulfix(__COS, temp0.x) + Mulfix(__SIN, temp0.z);
	__POINT0.position.z = Mulfix(__COS, temp0.z) - Mulfix(__SIN, temp0.x);

	__POINT1.position.x = Mulfix(__COS, temp1.x) + Mulfix(__SIN, temp1.z);
	__POINT1.position.z = Mulfix(__COS, temp1.z) - Mulfix(__SIN, temp1.x);

	__POINT2.position.x = Mulfix(__COS, temp2.x) + Mulfix(__SIN, temp2.z);
	__POINT2.position.z = Mulfix(__COS, temp2.z) - Mulfix(__SIN, temp2.x);
}

static inline void Projection() {
	//__POINT0.position.z = 65535 -  __POINT0.position.z;
	//__POINT1.position.z = 65535 -  __POINT1.position.z;
	//__POINT2.position.z = 65535 -  __POINT2.position.z;

	__POINT0.position.x = Divfix(__POINT0.position.x, 65535 - __POINT0.position.z);
	__POINT0.position.y = Divfix(__POINT0.position.y, 65535 - __POINT0.position.z);

	__POINT1.position.x = Divfix(__POINT1.position.x, 65535 - __POINT1.position.z);
	__POINT1.position.y = Divfix(__POINT1.position.y, 65535 - __POINT1.position.z);

	__POINT2.position.x = Divfix(__POINT2.position.x, 65535 - __POINT2.position.z);
	__POINT2.position.y = Divfix(__POINT2.position.y, 65535 - __POINT2.position.z);
}

static inline void Viewport() {
	__POINT0.position.x = UNFIX(__POINT0.position.x << 4) + LCD_WIDTH_PX / 2;
	__POINT0.position.y = LCD_HEIGHT_PX / 2 - UNFIX(__POINT0.position.y << 4);

	__POINT1.position.x = UNFIX(__POINT1.position.x << 4) + LCD_WIDTH_PX / 2;
	__POINT1.position.y = LCD_HEIGHT_PX / 2 - UNFIX(__POINT1.position.y << 4);

	__POINT2.position.x = UNFIX(__POINT2.position.x << 4) + LCD_WIDTH_PX / 2;
	__POINT2.position.y = LCD_HEIGHT_PX / 2 - UNFIX(__POINT2.position.y << 4);

	
}

static inline void Fragment(Vec3fix lightpos,Vec3fix objectcolor,Vec3fix lightcolor) {
	if (__POINT0.position.y == __POINT1.position.y && __POINT0.position.y == __POINT2.position.y)
		return;
	int t0x = __POINT0.position.x, t1x = __POINT1.position.x, t2x = __POINT2.position.x;
	int t0y = __POINT0.position.y, t1y = __POINT1.position.y, t2y = __POINT2.position.y;
	if (t0y > t1y) Swapi(t0y, t1y);
	if (t0y > t2y) Swapi(t0y, t2y);
	if (t1y > t2y) Swapi(t1y, t2y);
	if (t0x > t1x) Swapi(t0x, t1x);
	if (t0x > t2x) Swapi(t0x, t2x);
	if (t1x > t2x) Swapi(t1x, t2x);
	Vec3i P;
	for (P.x = t0x; P.x < t2x; P.x++) {
		for (P.y = t0y; P.y < t2y; P.y++) {
			Vec3fix bc_screen = Barycentric(__POINT0.position, __POINT1.position, __POINT2.position, P);
			P.z = Mulfix(bc_screen.x, __POINT0.position.z + __POINT1.position.z + __POINT2.position.z);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 || (ZBUF[P.x + P.y * LCD_WIDTH_PX]) > P.z) continue;
			Vec3fix Pnor;
			Pnor.x = Mulfix(bc_screen.x, __POINT0.normal.x + __POINT1.normal.x + __POINT2.normal.x);
			Pnor.y = Mulfix(bc_screen.y, __POINT0.normal.y + __POINT1.normal.y + __POINT2.normal.y);
			Pnor.z = Mulfix(bc_screen.z, __POINT0.normal.z + __POINT1.normal.z + __POINT2.normal.z);
			Vec3fix lightDir;
			lightDir.x = lightpos.x - (P.x);
			lightDir.y = lightpos.y - (P.y);
			lightDir.z = lightpos.z - (P.z);
			Normalizefix(&lightDir);
			Normalizefix(&Pnor);
			fix diff = abs(Dotfix(Pnor, lightDir));
			Vec3fix diffuse;
			diffuse.x = Mulfix(diff, objectcolor.x) * lightcolor.x >> 15;
			diffuse.x = diffuse.x * 255 >> 15;
			diffuse.y = Mulfix(diff, objectcolor.y) * lightcolor.y >> 15;
			diffuse.y = diffuse.y * 255 >> 15;
			diffuse.z = Mulfix(diff, objectcolor.z) * lightcolor.z >> 15;
			diffuse.z = diffuse.z * 255 >> 15;
			unsigned short endcolor = RGB_to_565(diffuse.x, diffuse.y, diffuse.z);
			DrawPoint_s(P.x, P.y, endcolor);
			ZBUF[P.x + P.y * LCD_WIDTH_PX] = P.z;
		}
	}

}

#endif

