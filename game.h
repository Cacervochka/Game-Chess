#pragma once

#include "config.h"
#include "board.h"
#include "input.h"

class Game {
    public:
        Game()
            : isRunning(false),
              gWindow(nullptr),
              gRenderer(nullptr),
              board(nullptr),
              previousSelected{-1, -1}
        {
            if(!init()){
                printf("Game initilize error!");
            }
            else{
                board = new Board(gRenderer);
                isRunning = true;
            }
        }
        ~Game()
        {
            delete board;

            //Destroy window	
            SDL_DestroyRenderer( gRenderer );
            SDL_DestroyWindow( gWindow );
            gWindow = NULL;
            gRenderer = NULL;

            //Quit SDL subsystems
            IMG_Quit();
            SDL_Quit();
        }

        bool init();
        void run();
    
    private:
        bool isRunning;
        //The window we'll be rendering to
        SDL_Window* gWindow;

        //The window gRenderer
        SDL_Renderer* gRenderer;
        
        Board* board;
        SDL_Point previousSelected;

        void handleEvents();
        void update();
        void render();
};