#include "SDL.h"

int main() {
    SDL_Init(0);

    SDL_CalculateGammaRamp(0, (void*)0);
    SDL_Quit();
    return 0;
}
