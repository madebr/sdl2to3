#pragma once

int SDL_Init(int arg);
void SDL_Quit(void);
void SDL_CalculateGammaRamp(float, unsigned short *);
void *SDL_CreateRGBSurface(unsigned, int, int, int, unsigned, unsigned, unsigned, unsigned);
void *SDL_CreateRGBSurfaceWithFormat(int, int, int, int, int);
void *SDL_CreateRGBSurfaceWithFormatFrom(void*, int, int, int, int, int);
void *SDL_CreateRGBSurfaceFrom(void*, int, int, int, int, int, int, int, int);
