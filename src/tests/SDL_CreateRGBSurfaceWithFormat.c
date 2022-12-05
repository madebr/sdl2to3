#include "SDL.h"

int an_arg(int a) {
    return 3;
}

void func() {
    SDL_CreateRGBSurfaceWithFormat(0x100, an_arg(1), 4+5, an_arg(2), 10);
    SDL_CreateRGBSurfaceWithFormat(an_arg(0), an_arg(1), an_arg(2), an_arg(3), an_arg(4));
}
