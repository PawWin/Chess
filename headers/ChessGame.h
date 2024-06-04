#ifndef CHESS_CHESSGAME_H
#define CHESS_CHESSGAME_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include "ChessPiece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
class ChessGame {
private:
    std::vector<std::vector<Piece*>> board; // 8x8 chess board
    bool isWhiteTurn; // Indicates whose turn it is
    std::vector<Piece::Move> moveHistory;
    std::pair<int, int> enPassantTarget;
    struct MoveAI {
        int fromX, fromY;
        int toX, toY;
    };
    //AI Opponent
    int evaluateBoard();
    int minimax(int depth, bool isMaximizingPlayer);
    MoveAI findBestMove(bool white);
public:
    ChessGame();
    ~ChessGame();



    void initializeBoard();
    bool isValidMove(const std::string& from, const std::string& to);
    void movePiece(const std::string& from, const std::string& to);
    void makeMove(bool AI);
    bool isValidInput(const std::string& from, const std::string& to);
    void displayBoard();
    void playGame();
    void PlayGameAI();
    void PlayerVsPlayer();
    bool isDraw();
    bool isInsufficientMaterial();
    bool isStalemate(bool white);
    bool isCheckmate(bool white);
    bool isCheck(bool byWhite);
    bool isSquareAttacked(int x, int y, bool byWhite, std::vector<std::vector<bool>>& visited);
    bool isValidSquare(int x, int y);
    void loadGame(const std::string& filename);
    void saveMovesToFile(const std::string& filename);
    void saveToFile(const std::string& filename);
    void resetBoard();
    void castle(bool isWhite, bool kingside);
    std::string toChessNotation(int x, int y);
    bool isValidEnPassant(const std::string& from, const std::string& to);
    void enPassantCapture(const std::string& from, const std::string& to);
    void printChessLogo();
    void promotePawn(const std::string& position);
    std::string convertToPosition(int row, int col);
    void makeAIMove();
    int evaluatePawnStructure();
    bool isCheckAI(bool white);


};

#endif // CHESS_CHESSGAME_H
