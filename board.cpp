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
    return false;
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
        fprintf(stderr, "Нельзя ходить! Ваш король будет под шахом.\n");
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