#pragma once

#include "config.h"
#include "pieces.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

void dumpBoard(Piece* (&gBoard)[8][8]);

class Board
{
    public:            
    
        enum PieceColor
        {
            WHITE=1,
            BLACK=-1
        };

        Board(SDL_Renderer* renderer)
        {   
            gRenderer=renderer;
            if(gRenderer==NULL)
            {
                printf("Error render area == NULL");
            }
            if(!initBoard()){
                printf("Failed to initializate board!");
            }
            colorTurn=WHITE;
        }

        bool initBoard()
        {
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (gBoard[i][j] != nullptr) {
                        delete gBoard[i][j];
                        gBoard[i][j] = nullptr;
                    }
                }
            }

            for (int j = 0; j < 8; ++j) {
                gBoard[1][j] = new Pawn(Piece::PAWN, Piece::BLACK, j, 1, gRenderer);
            }
            gBoard[0][0] = new Rook(Piece::ROOK, Piece::BLACK, 0, 0, gRenderer);
            gBoard[0][7] = new Rook(Piece::ROOK, Piece::BLACK, 7, 0, gRenderer);
            gBoard[0][1] = new Knight(Piece::KNIGHT, Piece::BLACK, 1, 0, gRenderer);
            gBoard[0][6] = new Knight(Piece::KNIGHT, Piece::BLACK, 6, 0, gRenderer);
            gBoard[0][2] = new Bishop(Piece::BISHOP, Piece::BLACK, 2, 0, gRenderer);
            gBoard[0][5] = new Bishop(Piece::BISHOP,Piece:: BLACK, 5, 0, gRenderer);
            gBoard[0][3] = new Queen(Piece::QUEEN, Piece::BLACK, 3, 0, gRenderer);
            gBoard[0][4] = new King(Piece::KING, Piece::BLACK, 4, 0, gRenderer);

            
            for (int j = 0; j < 8; ++j) {
                gBoard[6][j] = new Pawn(Piece::PAWN, Piece::WHITE, j, 6, gRenderer);
            }

            gBoard[7][0] = new Rook(Piece::ROOK,Piece::WHITE, 0, 7, gRenderer);
            gBoard[7][7] = new Rook(Piece::ROOK, Piece::WHITE, 7, 7, gRenderer);
            gBoard[7][1] = new Knight(Piece::KNIGHT, Piece::WHITE, 1, 7, gRenderer);
            gBoard[7][6] = new Knight(Piece::KNIGHT, Piece::WHITE, 6, 7, gRenderer);
            gBoard[7][2] = new Bishop(Piece::BISHOP, Piece::WHITE, 2, 7, gRenderer);
            gBoard[7][5] = new Bishop(Piece::BISHOP, Piece::WHITE, 5, 7, gRenderer);
            gBoard[7][3] = new Queen(Piece::QUEEN, Piece::WHITE, 3, 7, gRenderer);
            gBoard[7][4] = new King(Piece::KING, Piece::WHITE, 4, 7, gRenderer);

            return true;
        }

        bool isCheck(Piece::PieceColor kingColor);

        bool isCheckmate(Piece::PieceColor kingColor);

        bool handleMoveAttempt(SDL_Point from, SDL_Point to);

        bool isCastle(SDL_Point from, SDL_Point to);

        bool isEnPassant(SDL_Point from, SDL_Point to);

        bool isPiece(SDL_Point point);

        ~Board()
        {
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (gBoard[i][j] != nullptr) {
                        delete gBoard[i][j];
                    }
                }
            }
        }

        PieceColor getTurn()
        {
            return colorTurn;
        }

        void draw(SDL_Point selected);

    private:
        SDL_Renderer* gRenderer;
        Piece* gBoard[8][8]{};
        PieceColor colorTurn;

        SDL_Point previusMove;

        SDL_Point findKing(Piece::PieceColor kingColor) {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    Piece* piece = gBoard[y][x];
                    if (piece != nullptr && piece->getType() == Piece::KING && piece->getColor() == kingColor) {
                        return {x, y};
                    }
                }
            }
            return {-1, -1}; // На случай, если короля нет (хотя в шахматах такого быть не должно)
        }
};
