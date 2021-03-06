﻿#include <SDL.h>
#include <stdio.h>
#include "FXCG_RENDER.h"
#include "FXCG_STRING.h"


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

unsigned short empty[] = {
	0x528A,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0x632C,0xBDD7,
	0x528A,0x4A49,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0x5AEB,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x3100,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0xFD8A,0x9CD3,0x9CD3,0xB0E0,0xD000,0x9CD3,0x9CD3,0x3940,0x9CD3,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0xB0E0,0xB0E0,0x7080,0x8241,0xFCE2,0x7080,0xFD8A,0xFD8A,0xD000,0x9CD3,0x7080,0xB0E0,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0xD000,0xFD8A,0x9200,0xFCE2,0xE0A0,0xB0E0,0xFD8A,0x3100,0xCC60,0xB0E0,0xFCE2,0xCC60,0xFD8A,0xD000,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0xD000,0xFD8A,0xFCE2,0xCC60,0xE0A0,0x9200,0xCC60,0xFCE2,0xFD8A,0xD000,0x8241,0x9200,0xB0E0,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x7080,0xFD8A,0xD000,0x8241,0x3940,0xFD8A,0xE0A0,0x9200,0xFD8A,0xE0A0,0x3940,0x7080,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0xFD8A,0xFCE2,0xCC60,0xD000,0xF7E5,0x9200,0x9200,0x8241,0xD000,0x8241,0xFCE2,0xD000,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0xFD8A,0xE0A0,0x8241,0x3940,0xE0A0,0x8241,0xE0A0,0xE0A0,0xF7E5,0x3100,0x9200,0x7080,0xFD8A,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0xD000,0xFCE2,0xFD8A,0xF7E5,0xCC60,0xF7E5,0xD000,0xF7DF,0x8241,0x9200,0xCC60,0xD000,0x9CD3,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0xB0E0,0xB0E0,0xCC60,0x9200,0xFD8A,0xFCE2,0xD000,0xE0A0,0xE0A0,0x9200,0xFD8A,0x7080,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x7080,0xFD8A,0xD000,0x3940,0xD000,0xF7DF,0xF7E5,0x3100,0xCC60,0xF7E5,0x8241,0xD000,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0xFCE2,0x8241,0xD000,0xF7E5,0xD000,0xF7DF,0xF7E5,0xD000,0xFCE2,0xFD8A,0xB0E0,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x7080,0x3100,0x3940,0x3100,0x3940,0x20C1,0x28E0,0x20C0,0x28E0,0xB0E0,0xFCE2,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x3100,0x3940,0x3100,0x3940,0x3100,0x3940,0x20C1,0x28E0,0x20C0,0x28E0,0x20C0,0x28E0,0x20C0,0x28E0,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0xB596,0xBDD7,
	0x528A,0x4A49,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xB596,0xBDD7,
	0x528A,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xC618,0xBDD7
};



int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow("Soft3D", 500, 200, 384, 216, SDL_WINDOW_SHOWN);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 384, 216);

	VRAM_base = new unsigned short[384 * 216 * 2];
	ZBUF = new int[LCD_WIDTH_PX * LCD_HEIGHT_PX];
	Srand(50443);

	bool ApplicationDone = true;
	SDL_Event Event;
	SDL_Rect TextureRect = { 0,0,384,216 };
	int angle = 0;
	while (ApplicationDone) {
		while (SDL_PollEvent(&Event)) {
			if (Event.type == SDL_QUIT)
				ApplicationDone = false;
			if (Event.key.keysym.sym == SDLK_RIGHT)
				angle += 5;
		}
		
		unsigned int start = SDL_GetTicks();

		VRAMClear(0x0000);
		Memset4(ZBUF, INT_MIN, LCD_WIDTH_PX * LCD_HEIGHT_PX);

		Vec3fix lightpos = { 32768,32768,32768 };
		Vec3fix objectcolor = { 32768,0,0 };
		Vec3fix lightcolor = { 32768,32768,32768 };
		for (int i = 0; i < 6; i += 3) {
			ShaderInit(data[i], data[i + 1], data[i + 2]);
			RotationY(FIX(angle));
			Projection();
			Viewport();
			Fragment(lightpos, objectcolor,lightcolor);
		}

		unsigned int end = SDL_GetTicks();
		
		
		if (angle == 360)
			angle = 0;
		

		SDL_UpdateTexture(Texture, &TextureRect, VRAM_base, LCD_WIDTH_PX * 2);
		SDL_RenderCopy(Renderer, Texture, nullptr, &TextureRect);
		SDL_RenderPresent(Renderer);
	}

	delete[]VRAM_base;
	delete[]ZBUF;
	SDL_DestroyTexture(Texture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}