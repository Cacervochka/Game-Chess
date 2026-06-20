#pragma once

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>

class Piece {
    public:
        enum PieceColor {
            WHITE = 1,
            BLACK = -1
        };

        enum PieceType {
            EMPTY  = 0,
            PAWN   = 1,
            KNIGHT = 2,
            BISHOP = 3,
            ROOK   = 4,
            QUEEN  = 5,
            KING   = 6
        }; 

        Piece(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer)
            : type(type), color(color), x(x), y(y), gRenderer(renderer), texture(nullptr),isFirstMove(true)
        {
        }

        virtual bool isValidMove(int x, int y, Piece* (&gBoard)[8][8]) = 0;

        PieceColor getColor() const { return color; }
        PieceType getType() const { return type; }

        void setCoordinates(int x, int y)
        {
            this->x=x;
            this->y=y;
        }

        SDL_Point getCoordinates()
        {
            SDL_Point returnValue={this->x,this->y};
            return returnValue;
        }

        void firstMove()
        {
            this->isFirstMove=false;
        }

        bool getFirstMove()
        {
            return isFirstMove;
        }

        virtual bool loadTextureCheck() = 0;

        SDL_Texture* loadTexture(std::string path)
        {
            static std::map<std::string, SDL_Texture*> textureCache;

            // 2. Ищем, не загружали ли мы этот файл ранее?
            if (textureCache.find(path) != textureCache.end())
            {
                // Нашли! Возвращаем уже готовую текстуру, не трогая диск и память
                return textureCache[path];
            }

            SDL_Texture* newTexture = NULL;
            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if(loadedSurface == NULL)
            {
                printf("КРИТИЧЕСКАЯ ОШИБКА: Не могу найти файл по пути: %s!\n", path.c_str());
                printf("Текущая ошибка SDL_image: %s\n", IMG_GetError());
                
                // Принудительно завершаем процесс, чтобы не получить Segmentation fault дальше
                exit(1);
            }
            else
            {
                newTexture = SDL_CreateTextureFromSurface(this->gRenderer, loadedSurface);
                if(newTexture == NULL)
                {
                    printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
                }
                SDL_FreeSurface(loadedSurface);
                
            }

            if(newTexture == NULL)
            {
                printf("Ошибка создания текстуры из %s! SDL: %s\n", path.c_str(), SDL_GetError());
                exit(1);
            }

            // 4. Сохраняем текстуру в кэш для последующих вызовов
           if (newTexture != NULL) {
                textureCache[path] = newTexture;
            }

            return newTexture;
        }

        SDL_Texture* getTexture() const { return texture; }

        virtual ~Piece() {}
        
    protected:
        PieceType type;
        PieceColor color;
        int x;
        int y;
        SDL_Renderer* gRenderer;
        SDL_Texture* texture;
        bool isFirstMove;
};

class Rook : public Piece {
    public:
        Rook(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(ROOK,color,x,y,renderer) {            
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }
        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};

class Bishop : public Piece {
    public:
        Bishop(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(BISHOP,color,x,y,renderer) {
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }
        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};

class Queen : public Piece {
    public:
        Queen(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(QUEEN,color,x,y,renderer) {            
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }
        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};

class Pawn : public Piece {
    public:
        Pawn(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(PAWN,color,x,y,renderer) {
            
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }

        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};

class King : public Piece {
    public:
        King(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(KING,color,x,y,renderer) {
            
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }

        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};

class Knight : public Piece {
    public:
        Knight(PieceType type, PieceColor color, int x, int y, SDL_Renderer* renderer): Piece(KNIGHT,color,x,y,renderer) {
            
            if(!loadTextureCheck())
            {
                printf( "Failed to load piece textures!\n" );
            }
            
        }
        bool isValidMove(int x,int y, Piece* (&gBoard)[8][8]) override;
        bool loadTextureCheck() override;
};