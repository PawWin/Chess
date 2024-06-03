#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "ChessPiece.h"

class Bishop : public Piece {
public:
    Bishop(PieceColor color);

    bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
};

#endif // CHESS_BISHOP_H
