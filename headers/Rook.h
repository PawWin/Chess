#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "ChessPiece.h"

class Rook : public Piece {
public:
    Rook(PieceColor color);

     bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
};

#endif // CHESS_ROOK_H
