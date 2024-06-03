#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <vector>
#include <string>

class Piece {
public:
    enum PieceType {
        PAWN,
        BISHOP,
        KNIGHT,
        ROOK,
        QUEEN,
        KING,
    };

    enum PieceColor {
        WHITE,
        BLACK
    };
    struct Move {
        std::string from;
        std::string to;
    };

    Piece(PieceType type, PieceColor color);

    virtual ~Piece() {} ;


    std::string getTypeForBoard() const;

    PieceType getType() const;
    PieceColor getColor() const;

    // Pure virtual function to be implemented by each specific piece type
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) = 0;

protected:
    PieceType type;
    PieceColor color;
};

#endif // CHESS_PIECE_H
