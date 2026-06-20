#include "game.h"

bool Game::init()
{
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create gRenderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "gRenderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize gRenderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG; // Включаем основные декодеры
				if( ( IMG_Init( imgFlags ) & imgFlags ) != imgFlags )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void Game::update()
{

}

void Game::run() {
	const int FPS = 12;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

    while (isRunning) {
		frameStart = SDL_GetTicks(); // Время начала кадра
        handleEvents();
        update();
        render();

		frameTime = SDL_GetTicks() - frameStart; // Сколько времени ушло на кадр

		// Если кадр посчитался слишком быстро, отдыхаем остаток времени
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
    }
}


void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }

        SDL_Point pos = handleEvent(&e);
		printf("Pos: %d	%d\n",pos.x,pos.y);
        if (pos.x != -1 && pos.y != -1) {
            SDL_Point piecePos = {pos.x/(SCREEN_WIDTH/8),pos.y / (SCREEN_HEIGHT / 8)};
            if (previousSelected.x == -1 && previousSelected.y == -1) {
                if (board->isPiece(piecePos)) { 
                    previousSelected = pos;
                }
            } 
            else {
				SDL_Point from = {previousSelected.x/(SCREEN_WIDTH/8),previousSelected.y / (SCREEN_HEIGHT / 8)};
    			SDL_Point to = {pos.x / (SCREEN_WIDTH / 8),pos.y / (SCREEN_HEIGHT / 8)};
				printf("From: %d	%d\n To: %d		%d\n",from.x,from.y,to.x,to.y);
                board->handleMoveAttempt(from, to);
                previousSelected = {-1, -1};
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
	SDL_Point previusCell;
	if(previousSelected.x!=-1 && previousSelected.y!=-1){
		previusCell={previousSelected.x/(SCREEN_WIDTH/8),previousSelected.y / (SCREEN_HEIGHT / 8)};
	}
	else
	{
		previusCell={-1,-1};
	}

    board->draw(previusCell);

    SDL_RenderPresent(gRenderer);
}