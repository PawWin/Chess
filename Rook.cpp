#include "headers/Rook.h"

Rook::Rook(PieceColor color) : Piece(ROOK, color) {}

bool Rook::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    // Wieza rusza sie poziomo albo pionowo
    if (fromX != toX && fromY != toY) {
        return false; // Wieza nie moze poruszać sie po skosie
    }
    if (fromX == toX && fromY == toY) {
        return false; // Ta sama pozycja, błedny ruch
    }

    // Sprawdz czy sciezka jest pusta
    int xDir = (toX - fromX == 0) ? 0 : (toX - fromX > 0 ? 1 : -1);
    int yDir = (toY - fromY == 0) ? 0 : (toY - fromY > 0 ? 1 : -1);

    if (fromX != toX) {
        for (int i = 1; i < std::abs(toX - fromX); ++i) {
            if (board[fromX + i * xDir][fromY] != nullptr) {
                return false; // Sciezka zablokowana
            }
        }
    } else {
        for (int i = 1; i < std::abs(toY - fromY); ++i) {
            if (board[fromX][fromY + i * yDir] != nullptr) {
                return false; // Sciezka zablokowana
            }
        }
    }

    // Sprawdz czy pole docelowe jest puste badz zawiera figure przeciwnika
    if (board[toX][toY] == nullptr || board[toX][toY]->getColor() != color) {
        return true;
    }

    return false;
}

