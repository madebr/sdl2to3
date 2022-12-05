#pragma once

int SDL_Init(int);
void SDL_Quit(void);
void SDL_CalculateGammaRamp(float, unsigned short *);
int SDL_MasksToPixelFormatEnum(int, int, int, int, int);
void SDL_CreateSurface(int, int, int);
