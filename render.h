#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <map>
#include "pieces.h"

void render(int x,int y, SDL_Texture* texture, SDL_Renderer* gRenderer,
    const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

void renderPieceTextures(Piece* (&gBoard)[8][8],const int SCREEN_WIDTH,
    const int SCREEN_HEIGHT, SDL_Renderer* gRenderer);

void renderBoardTexture(Piece* (&gBoard)[8][8], 
                 const int SCREEN_WIDTH, const int SCREEN_HEIGHT, SDL_Renderer* gRenderer,
                 int selectedX, int selectedY);