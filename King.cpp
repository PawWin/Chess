#include "headers/King.h"

King::King(PieceColor color) : Piece(KING, color) {}

bool King::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    if (fromX == toX && fromY == toY) {
        return false; // Ta sama pozycja, błedny ruch
    }
    // Krol moze sie poruszac jedno pole w kazda strone
    int deltaX = std::abs(toX - fromX);
    int deltaY = std::abs(toY - fromY);

    if (deltaX <= 1 && deltaY <= 1) {
        // Sprawdz czy pole docelowe jest zajete przez friendly pionka
        if (board[toX][toY] != nullptr && board[toX][toY]->getColor() == color) {
            return false; // pole docelowe jest zajete przez friendly pionka, nielegalny ruch
        }
        return true; // legalny ruch
    }

    return false; // pole docelowe nie znajduje sie w zasiegu jednogo pola w zadna strone , nielegalny ruch
}
