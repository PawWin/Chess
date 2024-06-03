#include "headers/ChessPiece.h"

Piece::Piece(PieceType type, PieceColor color) : type(type), color(color) {}

std::string Piece::getTypeForBoard() const {
    std::string typeStr;
    switch (type) {
        case PAWN: typeStr = "P"; break;
        case BISHOP: typeStr = "B"; break;
        case KNIGHT: typeStr = "N"; break;
        case ROOK: typeStr = "R"; break;
        case QUEEN: typeStr = "Q"; break;
        case KING: typeStr = "K"; break;
        default: typeStr = "?";
    }

    return (color == WHITE) ? "W" + typeStr : "B" + typeStr;
}


Piece::PieceType Piece::getType() const {
    return type;
}

Piece::PieceColor Piece::getColor() const {
    return color;
}
