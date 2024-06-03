#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "ChessPiece.h"
#include <cmath>
#include <vector>
#include <iostream>
class Queen : public Piece {
public:
    Queen(PieceColor color);

     bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
};

#endif // CHESS_QUEEN_H
