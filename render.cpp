#include "render.h"

void render(int x,int y, SDL_Texture* texture, SDL_Renderer* gRenderer, 
    const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
	if (texture != nullptr) {
		SDL_Rect renderQuad = { x, y, SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8 };
		SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
	}
}

void renderPieceTextures(Piece* (&gBoard)[8][8],const int SCREEN_WIDTH,
    const int SCREEN_HEIGHT, SDL_Renderer* gRenderer)
{
	for(int i = 0; i<8;i++)
	{
		for (int j = 0; j<8;j++){

            if (gBoard[i][j] != nullptr) {
			    render(j * (SCREEN_WIDTH / 8), i * (SCREEN_HEIGHT / 8), gBoard[i][j]->getTexture(), 
                gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
            else{
                
            }
		}
	}
}

void renderBoardTexture(Piece* (&gBoard)[8][8], 
                        const int SCREEN_WIDTH, const int SCREEN_HEIGHT, SDL_Renderer* gRenderer,
                        int selectedX, int selectedY)
{
    //Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    int cellSizeX = SCREEN_WIDTH / 8;
    int cellSizeY = SCREEN_HEIGHT / 8;

    for(int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {   
            SDL_Rect cellRect;
            cellRect.x = j * cellSizeX;
            cellRect.y = i * cellSizeY;
            cellRect.w = cellSizeX;
            cellRect.h = cellSizeY;
            
            int pieceID = gBoard[i][j] != nullptr ? gBoard[i][j]->getType() : 0;

            if (pieceID != 0 && j == selectedX && i == selectedY)
            {
                SDL_SetRenderDrawColor(gRenderer, 0x7A, 0xC1, 0x7A, 0xFF); 
                SDL_RenderFillRect(gRenderer, &cellRect);
            }
            else
            {
                if ((i + j) % 2 == 0) 
                {
                    SDL_SetRenderDrawColor(gRenderer, 0xe6, 0xdb, 0xf1, 0xFF);
                }
                else 
                {
                    SDL_SetRenderDrawColor(gRenderer, 0x95, 0x7a, 0xb0, 0xFF); 
                }
                SDL_RenderFillRect(gRenderer, &cellRect);
            }
        }
    }

    renderPieceTextures(gBoard, SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);
}