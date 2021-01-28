#include <SDL.h>
#include <stdio.h>
#include "FXCG_RENDER.h"
#include "FXCG_STRING.h"




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
	while (ApplicationDone) {
		while (SDL_PollEvent(&Event)) {
			if (Event.type == SDL_QUIT)
				ApplicationDone = false;
		}

		unsigned int start = SDL_GetTicks();

		VRAMClear(Rand()%65535);
		Memset4(ZBUF, INT_MAX, LCD_WIDTH_PX * LCD_HEIGHT_PX);

		


		unsigned int end = SDL_GetTicks();
		printf("%d\n", end - start);

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