#include "headers/Queen.h"

Queen::Queen(PieceColor color) : Piece(QUEEN, color) {}

bool Queen::isValidMove(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Piece*>>& board) {
    if (fromX == toX && fromY == toY) {
        return false; // Ta sama pozycja, błedny ruch
    }
    // Spradz czy ruch jest horyzontalny, poziomy lub po skosie
    if ((std::abs(toX - fromX) == std::abs(toY - fromY)) || (fromX == toX || fromY == toY)) {

        // Ustal kierunek ruchu
        int xDir = (toX - fromX > 0) ? 1 : (toX - fromX < 0) ? -1 : 0;
        int yDir = (toY - fromY > 0) ? 1 : (toY - fromY < 0) ? -1 : 0;

        // Przejdz prez sciezke ruchu
        for (int i = 1; i <= std::max(std::abs(toX - fromX), std::abs(toY - fromY)); ++i) {
            int checkX = fromX + i * xDir;
            int checkY = fromY + i * yDir;

            // Sprawdz czy jakiekolwiek pole jest zajete
            if (board[checkX][checkY] != nullptr) {
                if (checkX == toX && checkY == toY) {
                    if (board[toX][toY]->getColor() != color) {
                        return true; // Pionek przeciwnika, legalny ruch
                    }else{
                        return false;
                    }
                } else {
                    return false; // Zajjete pole, nielegalny ruch
                }
            }
        }

        return true; // pusta sciezka, legalny ruch
    }

    return false;
}


