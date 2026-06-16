#include "pieces.h"

// Implementations of the isValidMove methods for each piece type
bool Rook::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    if(x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return false;
    }

    if(this->x == x && this->y == y)
    {
        return false;
    }

    if(this->x != x && this->y != y)
    {
        return false;
    }

    int stepX=(x-this->x==0) ? 0 : ((x-this->x) > 0 ? 1 : -1);
    int stepY=(y-this->y==0) ? 0 : ((y-this->y) > 0 ? 1 : -1);

    int tempX=this->x;
    int tempY=this->y;

    while(tempX != x || tempY != y)
    {
        if(gBoard[tempY][tempX] != nullptr)
        {
            return false;
        }             
        
        tempX += stepX;
        tempY += stepY;      
    }

    if(gBoard[y][x] != nullptr && gBoard[y][x]->getColor() == this->getColor())
    {
         return false;
    }


    return true;
}

bool Bishop::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    if(x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return false;
    }

    if(this->x == x && this->y == y)
    {
        return false;
    }

    if(abs(x - this->x) != abs(y - this->y))
    {
        return false;
    }

    int stepX = (x - this->x > 0) ? 1: -1;
    int stepY = (y - this->y > 0) ? 1: -1;

    int tempX=this->x+stepX;
    int tempY=this->y+stepY;

    while(tempX != x && tempY != y)
    {

        if(gBoard[tempY][tempX] != nullptr && tempX != x && tempY != y)
        {
            return false;
        }        
        
        tempX += stepX;
        tempY += stepY;
    }

    if(gBoard[y][x] != nullptr && (gBoard[y][x]->getColor() == this->getColor()))
    {
        return false;
    }

    return true;
}

bool Queen::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    bool isRookMove = (this->x == x || this->y == y);
    bool isBishopMove = (abs(x - this->x) == abs(y - this->y));

    if (!isRookMove && !isBishopMove) return false;

    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (this->x == x && this->y == y) return false;

    int stepX = (x - this->x == 0) ? 0 : ((x - this->x) > 0 ? 1 : -1);
    int stepY = (y - this->y == 0) ? 0 : ((y - this->y) > 0 ? 1 : -1);

    int tempX = this->x + stepX;
    int tempY = this->y + stepY;

    while (tempX != x || tempY != y)
    {
        if (gBoard[tempY][tempX] != nullptr) return false; 
        tempX += stepX;
        tempY += stepY;
    }

    if (gBoard[y][x] != nullptr && gBoard[y][x]->getColor() == this->getColor()) {
        return false;
    }

    return true;
}

bool Knight::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    if(x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return false;
    }

    if(this->x == x && this->y == y)
    {
        return false;
    }

    int newX = abs(x - this->x);
    int newY = abs(y - this->y);

    if(!((newX == 2 && newY == 1) || (newX == 1 && newY == 2)))
    {
        return false;
    }

    if(gBoard[y][x] != nullptr && (gBoard[y][x]->getColor() == this->getColor()))
    {
        return false;
    }

    return true;
}

bool King::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    if(x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return false;
    }

    if(this->x == x && this->y == y)
    {
        return false;
    }

    int newX = abs(x - this->x);
    int newY = abs(y - this->y);

    if(newX > 1 || newY > 1)
    {
        return false;
    }

    if(gBoard[y][x] != nullptr && (gBoard[y][x]->getColor() == this->getColor()))
    {
        return false;
    }

    return true;
}

bool Pawn::isValidMove(int x, int y, Piece* (&gBoard)[8][8])
{
    // 1. Базовые проверки границ доски и хода "на месте"
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (this->x == x && this->y == y) return false;

    // 2. Определяем правильное направление (для белых вверх, для черных вниз)
    // Предполагаем: Piece::WHITE == 1 (идет уменьшать Y), Piece::BLACK == -1 (идет увеличивать Y)
    int direction = (this->getColor() == Piece::WHITE) ? -1 : 1;

    int deltaX = x - this->x;
    int deltaY = y - this->y;

    // --- ВАРИАНТ А: Ход строго прямо (без взятия фигуры) ---
    if (deltaX == 0) 
    {
        // Ход на 1 клетку вперед
        if (deltaY == direction) 
        {
            if (gBoard[y][x] != nullptr) return false; // Клетка должна быть пустой
            return true;
        }
        
        // Ход на 2 клетки вперед (только если это первый ход)
        if (deltaY == 2 * direction && this->isFirstMove) 
        {
            // Проверяем клетку прямо перед пешкой и финальную клетку
            int stepY = this->y + direction;
            if (gBoard[stepY][x] != nullptr || gBoard[y][x] != nullptr) {
                return false; 
            }
            return true;
        }
        
        return false; // Любые другие шаги прямо (на 3 клетки или назад) запрещены
    }

    // --- ВАРИАНТ Б: Удар по диагонали (взятие фигуры) ---
    if (abs(deltaX) == 1 && deltaY == direction) 
    {
        // На целевой клетке обязательно должен стоять враг
        if (gBoard[y][x] != nullptr && gBoard[y][x]->getColor() != this->getColor()) {
            return true;
        }
        
        // (Опционально здесь можно будет позже запрограммировать взятие на проходе — En Passant)
        return false; 
    }

    // Все остальные траектории для пешки нелегальны
    return false;
}

bool Rook::loadTextureCheck()
{
    std::string texturePath = (getColor() == WHITE) ? "textures/White/Rook-White.png" : 
    "textures/Black/Rook-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}

bool Bishop::loadTextureCheck()
{
    std::string texturePath = (getColor() == 1) ? "textures/White/Bishop-White.png" : 
    "textures/Black/Bishop-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}

bool Queen::loadTextureCheck()
{
    std::string texturePath = (getColor() == 1) ? "textures/White/Queen-White.png" : 
    "textures/Black/Queen-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}

bool Knight::loadTextureCheck()
{
    std::string texturePath = (getColor() == 1) ? "textures/White/Knight-White.png" : 
    "textures/Black/Knight-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}

bool King::loadTextureCheck()
{
    std::string texturePath = (getColor() == 1) ? "textures/White/King-White.png" : 
    "textures/Black/King-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}

bool Pawn::loadTextureCheck()
{
    std::string texturePath = (getColor() == 1) ? "textures/White/Pawn-White.png" : 
    "textures/Black/Pawn-Black.png";
    texture = loadTexture(texturePath);
    return texture != nullptr ? true : false;
}