#include "SDL.h"

int an_arg(int a) {
    return 3;
}

void func() {
    SDL_CreateRGBSurface(0x100, an_arg(1), 4+5, an_arg(2), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_CreateRGBSurface(an_arg(0), an_arg(1), an_arg(2), an_arg(3), an_arg(4), an_arg(5), an_arg(6), an_arg(7));
}
