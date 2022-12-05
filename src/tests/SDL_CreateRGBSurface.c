#include "SDL.h"

int an_arg(int a) {
    return 3;
}

void func() {
    SDL_CreateRGBSurface(0x100, an_arg(1), 4+5, an_arg(2), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), an_arg(3), an_arg(4), an_arg(5), an_arg(6), an_arg(7));
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 32, 0xFF000000U, 0x00FF0000U, 0x0000FF00U, 0x000000FFU);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 32, 0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 16, 0x0000F800, 0x000007E0, 0x0000001F, 0x00000000);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), 16, (0x0000F000), 0x00000F00, 0x000000F0, 0x0000000F);
}
