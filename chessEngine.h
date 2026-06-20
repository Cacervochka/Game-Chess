#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

const int INF=1;
const int MATE_SCORE=0.1;

struct Move
{
    int fromX,fromY;
    int toX,toY;
    int score = 0;
};

int evaluateBoard() {
    // Здесь будет ваш двойной цикл по gBoard, считающий фигуры.
    // Возвращает плюс для Белых, минус для Черных.
    return 0; 
}

std::vector<Move> generateLegalMoves(bool isWhite) {
    std::vector<Move> moves;
    // Здесь ваша логика, которая собирает все легальные ходы для текущего игрока
    return moves;
}

void makeMove(Move move) { /* Временно двигает фигуру на доске */ }
void undoMove(Move move) { /* Возвращает фигуру обратно */ }
bool isCurrentKingInCheck() { /* Проверяет, под шахом ли король */ }

