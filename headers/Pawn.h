#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "ChessPiece.h"

class Pawn : public Piece {
public:
    Pawn(PieceColor color);

    bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) override;
    bool isEnPassantMove(const std::string& from, const std::string& to, const std::pair<int, int>& enPassantTarget) const;
};

#endif // CHESS_PAWN_H
