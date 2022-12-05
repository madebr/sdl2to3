#include "SDL.h"

int an_arg(int a) {
    return 3;
}

void func() {
    SDL_CreateRGBSurfaceFrom((void*)0x100, an_arg(1), 4+5, an_arg(2), an_arg(4), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_CreateRGBSurfaceFrom((void*)an_arg(0), an_arg(1), an_arg(2), an_arg(3), an_arg(4), an_arg(5), an_arg(6), an_arg(7), an_arg(8));
}
