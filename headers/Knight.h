#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "ChessPiece.h"

class Knight : public Piece {
public:
    Knight(PieceColor color);

     bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
};

#endif // CHESS_KNIGHT_H
