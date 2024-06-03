#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "ChessPiece.h"

class King : public Piece {
public:
    King(PieceColor color);

     bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
};

#endif // CHESS_KING_H
