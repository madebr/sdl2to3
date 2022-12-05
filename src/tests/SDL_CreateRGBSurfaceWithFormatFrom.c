#include "SDL.h"

int an_arg(int a) {
    return 3;
}

void func() {
    SDL_CreateRGBSurfaceWithFormatFrom((void*)0, an_arg(1), 4+5, an_arg(2), 10, 9);
    SDL_CreateRGBSurfaceWithFormatFrom((void*)an_arg(0), an_arg(1), an_arg(2), an_arg(3), an_arg(4), an_arg(5));
}
