#include "input.h"

SDL_Point handleEvent(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
    {
        const int cellX = e->button.x / (SCREEN_WIDTH / 8);
        const int cellY = e->button.y / (SCREEN_HEIGHT / 8);

        if (cellX >= 0 && cellX < 8 && cellY >= 0 && cellY < 8)
        {
            return SDL_Point{cellX, cellY};
        }
    }

    return SDL_Point{-1, -1};
}

