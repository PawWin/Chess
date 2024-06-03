#include "headers/Knight.h"

Knight::Knight(PieceColor color) : Piece(KNIGHT, color) {}

bool Knight::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    if (fromX == toX && fromY == toY) {
        return false; // Same position, invalid move
    }
    // Knight moves in an L-shape: two squares in one direction and then one square perpendicular to that direction
    int deltaX = std::abs(toX - fromX);
    int deltaY = std::abs(toY - fromY);

    // Check if the destination square is empty or occupied by an opponent's piece
    if (board[toX][toY] == nullptr || board[toX][toY]->getColor() != getColor()) {
        return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
    }

    return false;
}

