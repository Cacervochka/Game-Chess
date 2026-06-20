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
        movingPiece->getColor() != static_cast<Piece::PieceColor>(colorTurn)) {
        return false;
    }

    if(movingPiece->getType()==1)
    {
        if(isEnPassant(from,to))
        {
            gBoard[from.y][from.x] = nullptr;
            gBoard[to.y][to.x] = movingPiece;
            delete gBoard[from.y][to.x];
            gBoard[from.y][to.x]=nullptr;

            movingPiece->setCoordinates(to.x, to.y);
            movingPiece->firstMove();

            colorTurn = (colorTurn == WHITE) ? BLACK : WHITE;
    
            dumpBoard(gBoard);

            previusMove = to;

            return true;
        }
    }

    if(movingPiece->getType()==6)
    {
        if(isCastle(from,to))
        {   
            dumpBoard(gBoard);
            if(to.x==2)
            {
                Piece* KING=gBoard[from.y][from.x];
                Piece* ROOK=gBoard[from.y][0];
                gBoard[to.y][to.x]=KING;
                gBoard[from.y][3]=ROOK;
                gBoard[from.y][0]=nullptr;
                gBoard[from.y][from.x]=nullptr;
                
                KING->setCoordinates(to.x,to.y);
                ROOK->setCoordinates(3,from.y);

                KING->firstMove();
                ROOK->firstMove();
            }
            else if(to.x==6)
            {
                Piece* KING=gBoard[from.y][from.x];
                Piece* ROOK=gBoard[from.y][7];
                gBoard[to.y][to.x]=KING;
                gBoard[from.y][5]=ROOK;
                gBoard[from.y][7]=nullptr;
                gBoard[from.y][from.x]=nullptr;

                KING->setCoordinates(to.x,to.y);
                ROOK->setCoordinates(5,from.y);

                KING->firstMove();
                ROOK->firstMove();
            }
            dumpBoard(gBoard);
            colorTurn = (colorTurn == WHITE) ? BLACK : WHITE;
            return true;
        }
    }

    if(!movingPiece->isValidMove(to.x, to.y, gBoard)){return false;}

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

    previusMove = to;

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

bool Board::isCastle(SDL_Point from, SDL_Point to)
{
    Piece* KING = gBoard[from.y][from.x];
    if (KING == nullptr || !KING->getFirstMove()) return false;
    if (from.y != to.y) return false;

    if (isCheck(KING->getColor())) return false;

    if (to.x == 2) {
        if (gBoard[from.y][0] == nullptr || gBoard[from.y][1] != nullptr 
            || gBoard[from.y][2] != nullptr || gBoard[from.y][3] != nullptr) return false;

        if (!gBoard[from.y][0]->getFirstMove()) return false;

        int fieldsToCheck[] = {3, 2};
        for (int nextX : fieldsToCheck) {
            gBoard[from.y][from.x] = nullptr;
            gBoard[from.y][nextX] = KING;

            bool isThreatened = isCheck(KING->getColor());

            gBoard[from.y][nextX] = nullptr;
            gBoard[from.y][from.x] = KING;

            if (isThreatened) return false;
        }
    }
    else if (to.x == 6) {
        if (gBoard[from.y][7] == nullptr || gBoard[from.y][6] != nullptr 
            || gBoard[from.y][5] != nullptr) return false;

        if (!gBoard[from.y][7]->getFirstMove()) return false;

        int fieldsToCheck[] = {5, 6};
        for (int nextX : fieldsToCheck) {
            gBoard[from.y][from.x] = nullptr;
            gBoard[from.y][nextX] = KING;

            bool isThreatened = isCheck(KING->getColor());

            gBoard[from.y][nextX] = nullptr;
            gBoard[from.y][from.x] = KING;

            if (isThreatened) return false;
        }
    }
    else {
        return false;
    }

    return true; 
}

bool Board::isEnPassant(SDL_Point from, SDL_Point to)
{
    Piece* target=gBoard[from.y][from.x];

    if(target->getType()!=1){return false;}    
    
    if(from.x == to.x || abs(from.x-to.x)>1)
    {
        return false;
    }

    if((target->getColor()==1 && from.y!=3) || (target->getColor()==-1 && from.y!=4))
    {
        return false;
    }

    if(gBoard[from.y][to.x]==nullptr || gBoard[from.y][to.x]->getType()!=1)
    {
        return false;
    }

    if(from.y!=previusMove.y || to.x!=previusMove.x)
    {
        return false;
    }

    Piece* targetBackup = gBoard[from.y][to.x];

    gBoard[from.y][from.x] = nullptr;
    gBoard[to.y][to.x] = target;

    gBoard[from.y][to.x]=nullptr;

    bool isThreatened = isCheck(target->getColor());

    gBoard[from.y][to.x]=targetBackup;

    gBoard[from.y][from.x] = target;
    gBoard[to.y][to.x] = nullptr;

    if (isThreatened) return false;

    return true;

}