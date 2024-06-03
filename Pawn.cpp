#include "headers/Pawn.h"

Pawn::Pawn(PieceColor color) : Piece(PAWN, color) {}

bool Pawn::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    if (fromX == toX && fromY == toY) {
        return false; // Ta sama pozycja, błedny ruch
    }
    int direction = (color == WHITE) ? 1 : -1;
    // Porusz sie do przodu
    if (fromY == toY) {
        if (toX == fromX + direction && board[toX][toY] == nullptr) {
            return true;
        }
        if ((fromX == 1 && color == WHITE) || (fromX == 6 && color == BLACK)) {
            // Sprawdz czy pola pomiedzy sa puste
            if (toX == fromX + 2 * direction && board[fromX + direction][toY] == nullptr && board[fromX + 2 * direction][toY] == nullptr) {
                return true; // Porusz sie 2 pola do przodu
            }
        }

    }


    // Zbicie
    if (std::abs(fromY - toY) == 1 && toX == fromX + direction && board[toX][toY] != nullptr) {
        // Upewnij sie ze zbijasz pionka przeciwnika
        if (board[toX][toY]->getColor() != color) {
            return true;
        }
    }

    return false;
}

