#include "FXCG_RENDER.h"
#include "FXCG_STRING.h"
#include "FXCG_KEYBOARD.h"
#include <fxcg/rtc.h>

Point3 data[6] = {
		//================================================
		//========Position========normal==========uv======
		//================================================
		-16384, -16384, 16384,  0, 0, -32768, 32768, 32768,
		 16384, -16384, 16384,  0, 0, -32768, 0,     32768,
		 16384,  16384, 16384,  0, 0, -32768, 0,     0,
		 16384,  16384, -16384,  0, 0, -32768, 0,     0,
		-16384,  16384, -16384,  0, 0, -32768, 32768, 0,
		-16384, -16384, -16384,  0, 0, -32768, 32768, 32768
};

int main() {
	VRAM_base=(color_t*)GetVRAMAddress();
	Bdisp_EnableColor(1);
	EnableStatusArea(3);

	fix zbuf[384*216];
	ZBUF=zbuf;
	char buf[12];
	bool done=true;
	int angle = 0;
	Vec3fix lightpos = { 32768,32768,32768 };
	Vec3fix objectcolor = { 32768,0,0 };
	Vec3fix lightcolor = { 32768,32768,32768 };
	while(done) {
		if(*((unsigned short*)0xA44B0008) == 0x8)//MENU��
			done = false;
		if(*((unsigned short*)0xA44B0008) == 0x4)
			angle+=5;
		
		VRAMClear(0x0000);
		Memset4(ZBUF,INT_MIN,384*216);
		for (int i = 0; i < 6; i += 3) {
			ShaderInit(data[i], data[i + 1], data[i + 2]);
			RotationY(FIX(angle));
			Projection();
			Viewport();
			Fragment(lightpos, objectcolor,lightcolor);
		}

		if (angle==360)
			angle=0;

		PutDisplay();
		PutDisplayWait();
	}
	
	return 0;
}