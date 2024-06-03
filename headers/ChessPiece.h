#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <vector>
#include <string>

class Piece {
public:
    //Typ pionka
    enum PieceType {
        PAWN,
        BISHOP,
        KNIGHT,
        ROOK,
        QUEEN,
        KING,
    };
    //Kolor Pionka
    enum PieceColor {
        WHITE,
        BLACK
    };
    //Ruch
    struct Move {
        std::string from;
        std::string to;
    };
    //Konstruktor
    Piece(PieceType type, PieceColor color);

    virtual ~Piece() {} ;


    std::string getTypeForBoard() const;

    PieceType getType() const;
    PieceColor getColor() const;

    // Wirtualan funkcja do nadpisania w kazdej klasie pionka
    virtual bool isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) = 0;

protected:
    PieceType type;
    PieceColor color;
};

#endif // CHESS_PIECE_H
