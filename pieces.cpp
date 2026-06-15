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
        tempX += stepX;
        tempY += stepY;

        if(gBoard[tempY][tempX] != nullptr && tempX != x && tempY != y)
        {
            return false;
        }
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
        printf("False out of range\n");
        return false;
    }

    if(this->x == x && this->y == y)
    {
        printf("False x==x,y==y\n");
        return false;
    }

    if(abs(x - this->x) != abs(y - this->y))
    {
        printf("False not on diagonal\n");
        return false;
    }

    int stepX = (x - this->x > 0) ? 1: -1;
    int stepY = (y - this->y > 0) ? 1: -1;

    int tempX=this->x;
    int tempY=this->y;

    printf("Step X: %d, Step Y: %d \n", stepX,stepY);
    while(tempX != x && tempY != y)
    {
        tempX += stepX;
        tempY += stepY;

        if(gBoard[tempY][tempX] != nullptr && tempX != x && tempY != y)
        {
            printf("False,piece on the diagonal\n");
            return false;
        }
    }

    if(gBoard[y][x] != nullptr && (gBoard[y][x]->getColor() == this->getColor()))
    {
        printf("False,colors\n");
        return false;
    }

    printf("True\n");
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

    if(gBoard[y][x] != nullptr && gBoard[y][x]->getColor() != this->getColor())
    {
        return true;
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

bool Pawn::isValidMove(int x,int y, Piece* (&gBoard)[8][8])
{
    if(x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        return false;
    }

    if(this->x == x && this->y == y)
    {
        return false;
    }

    if(this->y==y && gBoard[y][x]!=nullptr)
    {
        return false;
    }

    int newY = abs(y - this->y);
    int newX = abs(x - this->x);
    
    if((this->color==1 && y>this->y) || (this->color==-1 && y<this->y))
    {
        return false;
    }

    if(newX>1 || newY > 2)
    {
        return false;
    }
    
    if(newY==2 && newX==1)
    {
        return false;
    }

    if(newY == 2 && !this->isFirstMove
    )
    {
        return false;
    }

    if(newX == 1 && newY == 1)
    {
        if(gBoard[y][x] != nullptr && gBoard[y][x]->getColor() != this->getColor())
        {
            this->isFirstMove = false;
            return true;
        }

        return false;
    }
    else if(newX == 0 && newY == 1)
    {
        if(gBoard[y][x] != nullptr)
        {
            return false;
        }
    }

    return true;
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