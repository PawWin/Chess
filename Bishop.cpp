#include "headers/Bishop.h"

Bishop::Bishop(PieceColor color) : Piece(BISHOP, color) {}

bool Bishop::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    // Bishop moze sie poruszac tylko po skosie
    if (std::abs(toX - fromX) != std::abs(toY - fromY)) {
        return false;
    }
    if (fromX == toX && fromY == toY) {
        return false; // Ta sama pozycja, błedny ruch
    }

    // Sprawdz czy jakiekolwiek pionki blokują drogę
    int xDir = (toX - fromX) > 0 ? 1 : -1;
    int yDir = (toY - fromY) > 0 ? 1 : -1;

    for (int i = 1; i < std::abs(toX - fromX); ++i) {
        if (board[fromX + i * xDir][fromY + i * yDir] != nullptr) {
            return false; // Droga jest zablokowana
        }
    }

    // Sprawdz czy pola docelowe zawiera pionki przeciwnika albo jest puste
    if (board[toX][toY] == nullptr || board[toX][toY]->getColor() != color) {
        return true;
    }

    return false;
}
