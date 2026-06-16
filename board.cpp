#include "board.h"
#include "render.h"

bool Board::isCheck(Piece::PieceColor kingColor)
{
    SDL_Point kingPos = findKing(kingColor);
    if (kingPos.x == -1 || kingPos.y == -1) {
        return false; 
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = gBoard[y][x];

            if (piece != nullptr && piece->getColor() != kingColor) {
                
                if (piece->isValidMove(kingPos.x, kingPos.y, gBoard)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::isCheckmate(Piece::PieceColor kingColor)
{
    if (!isCheck(kingColor)) {
        return false; 
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = gBoard[y][x];

            if (piece != nullptr && piece->getColor() == kingColor) {
                
                for (int targetY = 0; targetY < 8; ++targetY) {
                    for (int targetX = 0; targetX < 8; ++targetX) {
                        
                        if (piece->isValidMove(targetX, targetY, gBoard)) {
                            
                            Piece* savedTargetPiece = gBoard[targetY][targetX];
                            
                            gBoard[targetY][targetX] = piece;
                            gBoard[y][x] = nullptr;
                            piece->setCoordinates(targetX, targetY);

                            if (savedTargetPiece != nullptr) {
                                savedTargetPiece->setCoordinates(-1, -1);
                            }

                            bool stillInCheck = isCheck(kingColor);

                            gBoard[y][x] = piece;
                            gBoard[targetY][targetX] = savedTargetPiece;
                            piece->setCoordinates(x, y);
                            
                            if (savedTargetPiece != nullptr) {
                                savedTargetPiece->setCoordinates(targetX, targetY);
                            }

                            if (!stillInCheck) {
                                return false; 
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Board::handleMoveAttempt(SDL_Point from, SDL_Point to)
{
    if (from.x < 0 || from.x >= 8 || from.y < 0 || from.y >= 8 ||
        to.x < 0 || to.x >= 8 || to.y < 0 || to.y >= 8) {
        return false;
    }

    Piece* movingPiece = gBoard[from.y][from.x];
    if (movingPiece == nullptr ||
        movingPiece->getColor() != static_cast<Piece::PieceColor>(colorTurn) ||
        !movingPiece->isValidMove(to.x, to.y, gBoard)) {
        return false;
    }

    Piece* targetBackup = gBoard[to.y][to.x];
    
    gBoard[to.y][to.x] = movingPiece;
    gBoard[from.y][from.x] = nullptr;
    SDL_Point oldPoint = movingPiece->getCoordinates();    
    int oldX = oldPoint.x;
    int oldY = oldPoint.y;
    movingPiece->setCoordinates(to.x, to.y);

    bool isOwnKingUnderCheck = isCheck(movingPiece->getColor());

    gBoard[from.y][from.x] = movingPiece;
    gBoard[to.y][to.x] = targetBackup;
    movingPiece->setCoordinates(oldX, oldY);

    if (isOwnKingUnderCheck) {        
        if(isCheckmate(movingPiece->getColor()))
        {
            fprintf(stderr,"Шах и мат! \n");
            printf("Color: %d\n",movingPiece->getColor());
            

            return false;
        }
        fprintf(stderr, "Нельзя ходить! Ваш король будет под шахом.\n");
        printf("Color: %d\n",movingPiece->getColor());

        dumpBoard(gBoard);

        return false;
    }

    if (gBoard[to.y][to.x] != nullptr) {
        delete gBoard[to.y][to.x];
    }
    
    gBoard[to.y][to.x] = movingPiece;
    movingPiece->setCoordinates(to.x, to.y);
    movingPiece->firstMove();
    gBoard[from.y][from.x] = nullptr;

    colorTurn = (colorTurn == WHITE) ? BLACK : WHITE;
    
    dumpBoard(gBoard);

    return true;
}

bool Board::isPiece(SDL_Point point)
{
    if (point.x < 0 || point.x >= 8 || point.y < 0 || point.y >= 8) {
        return false;
    }

    return gBoard[point.y][point.x] != nullptr;
}

void Board::draw(SDL_Point selected)
{
    renderBoardTexture(gBoard,SCREEN_WIDTH,SCREEN_HEIGHT,gRenderer,selected.x,selected.y);
}

void dumpBoard(Piece* (&gBoard)[8][8]) 
{
    fprintf(stderr, "\n--- ТЕКУЩЕЕ СОСТОЯНИЕ ДОСКИ ---\n");
    for (int i = 0; i < 8; i++) 
    {
        fprintf(stderr, "%d | ", 8 - i); // Номер строки для удобства (от 8 до 1)
        for (int j = 0; j < 8; j++) 
        {
            if (gBoard[i][j] == nullptr) {
                fprintf(stderr, ". "); // Пустая клетка
            } else {
                // Определяем букву фигуры
                char symbol = '?';
                switch (gBoard[i][j]->getType()) {
                    case Piece::PAWN:   symbol = 'P'; break;
                    case Piece::KNIGHT: symbol = 'N'; break;
                    case Piece::BISHOP: symbol = 'B'; break;
                    case Piece::ROOK:   symbol = 'R'; break;
                    case Piece::QUEEN:  symbol = 'Q'; break;
                    case Piece::KING:   symbol = 'K'; break;
                    default:            symbol = '?'; break;
                }
                
                // Если фигура черная, сделаем её строчной буквой (как в шахматах)
                if (gBoard[i][j]->getColor() == Piece::BLACK) {
                    symbol = tolower(symbol);
                }
                
                fprintf(stderr, "%c ", symbol);
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "    -----------------\n");
    fprintf(stderr, "    a b c d e f g h\n\n");
}