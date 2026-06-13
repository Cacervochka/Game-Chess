//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <map>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Loads the chess board
bool loadBoard();

//Loads piece textures
bool loadPieceTextures();

void render(int x,int y, SDL_Texture* texture);

void renderBoard();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

std::map<int, SDL_Texture*> gPieceTextures;

enum PieceType {
    EMPTY  = 0,
    PAWN   = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK   = 4,
    QUEEN  = 5,
    KING   = 6
};

int gBoard[8][8] = {
    {-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK}, // Черные фигуры
    {-PAWN, -PAWN,   -PAWN,   -PAWN,  -PAWN, -PAWN,   -PAWN,   -PAWN}, // Черные пешки
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {PAWN,  PAWN,    PAWN,    PAWN,   PAWN,  PAWN,    PAWN,    PAWN},  // Белые пешки
    {ROOK,  KNIGHT,  BISHOP,  QUEEN,  KING,  BISHOP,  KNIGHT,  ROOK}   // Белые фигуры
};

bool init()
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
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

bool loadBoard()
{

	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	//Render of black and white squares
	SDL_Rect fillRect[64];
	for( int i = 0; i < 64; ++i )
	{
		fillRect[i].x = ( i % 8 ) * ( SCREEN_WIDTH / 8 );
		fillRect[i].y = ( i / 8 ) * ( SCREEN_HEIGHT / 8 );
		fillRect[i].w = SCREEN_WIDTH / 8;
		fillRect[i].h = SCREEN_HEIGHT / 8;

		if( ( i + i / 8 ) % 2 == 0 )
		{
			SDL_SetRenderDrawColor( gRenderer,  0xe6,0xdb,0xf1, 0xFF);		
			SDL_RenderFillRect( gRenderer, &fillRect[i] );
		}
		else
		{
			SDL_SetRenderDrawColor( gRenderer, 0x95, 0x7a, 0xb0, 0xFF);		
			SDL_RenderFillRect( gRenderer, &fillRect[i] );
		}
	}	
	return true;

}

bool loadPieceTextures()
{
	try
	{
		// Черные фигуры (отрицательные ID)
        gPieceTextures[-PAWN]   = loadTexture("textures/black/Pawn-Black.png");
        gPieceTextures[-KNIGHT] = loadTexture("textures/black/Knight-Black.png");
        gPieceTextures[-BISHOP] = loadTexture("textures/black/Bishop-Black.png");
        gPieceTextures[-ROOK]   = loadTexture("textures/black/Rook-Black.png");
        gPieceTextures[-QUEEN]  = loadTexture("textures/black/Queen-Black.png");
        gPieceTextures[-KING]   = loadTexture("textures/black/King-Black.png");

        // Белые фигуры (положительные ID)
        gPieceTextures[PAWN]    = loadTexture("textures/white/Pawn-White.png");
        gPieceTextures[KNIGHT]  = loadTexture("textures/white/Knight-White.png");
        gPieceTextures[BISHOP]  = loadTexture("textures/white/Bishop-White.png");
        gPieceTextures[ROOK]    = loadTexture("textures/white/Rook-White.png");
        gPieceTextures[QUEEN]   = loadTexture("textures/white/Queen-White.png");
        gPieceTextures[KING]    = loadTexture("textures/white/King-White.png");
        
        // Для пустой клетки текстура не нужна
        gPieceTextures[EMPTY]   = nullptr;
	}
	catch (const std::exception& e)
	{
		printf( "Failed to load piece textures! Error: %s\n", e.what() );
		return false;
	}

	//Load piece textures
	return true;
}

void render(int x,int y, SDL_Texture* texture)
{
	if (texture != nullptr) {
		SDL_Rect renderQuad = { x, y, SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8 };
		SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
	}
}

void renderBoard()
{
	for(int i = 0; i<8;i++)
	{
		for (int j = 0; j<8;j++)
		{	
			int pieceID = gBoard[i][j];
			render(j * (SCREEN_WIDTH / 8), i * (SCREEN_HEIGHT / 8), gPieceTextures[pieceID]);
		}
	}
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			if(!loadBoard() || !loadPieceTextures()){	
				printf( "Failed to load board!\n" );
			}
			else
			{
				
				//Main loop flag
				bool quit = false;

				//Event handler
				SDL_Event e;
				//While application is running
				while( !quit )
				{
					//Handle events on queue
					while( SDL_PollEvent( &e ) != 0 )
					{
						//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
					}
					renderBoard();
					//Update screen
					SDL_RenderPresent( gRenderer );
				}
			}
		}
		
	}

	//Free resources and close SDL
	close();

	return 0;
}
