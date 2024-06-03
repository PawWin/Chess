#include "headers/ChessGame.h"

const int MAX_DEPTH = 4;
ChessGame::ChessGame() : board(8, std::vector<Piece*>(8, nullptr)), isWhiteTurn(true) {
    initializeBoard();
}

ChessGame::~ChessGame() {
    // Zwolnij pamięć dla figur
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }
}

void ChessGame::initializeBoard() {
    // Umieść białe figury
    board[0][0] = new Rook(Piece::WHITE);
    board[0][1] = new Knight(Piece::WHITE);
    board[0][2] = new Bishop(Piece::WHITE);
    board[0][3] = new Queen(Piece::WHITE);
    board[0][4] = new King(Piece::WHITE);
    board[0][5] = new Bishop(Piece::WHITE);
    board[0][6] = new Knight(Piece::WHITE);
    board[0][7] = new Rook(Piece::WHITE);

    // Umieść czarne figury
    board[7][0] = new Rook(Piece::BLACK);
    board[7][1] = new Knight(Piece::BLACK);
    board[7][2] = new Bishop(Piece::BLACK);
    board[7][3] = new Queen(Piece::BLACK);
    board[7][4] = new King(Piece::BLACK);
    board[7][5] = new Bishop(Piece::BLACK);
    board[7][6] = new Knight(Piece::BLACK);
    board[7][7] = new Rook(Piece::BLACK);

    // Umieść pionki
    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(Piece::WHITE);
        board[6][i] = new Pawn(Piece::BLACK);
    }
}

void ChessGame::playGame() {
    std::cout << "Welcome to Chess writen in C++ " << std::endl;
    printChessLogo();
    isWhiteTurn = true;
    // Zapytaj gracza, czy chce wczytać grę czy rozpocząć nową
    char choice;
    int input;
    do {
        std::cout << "1.Do you want to load a game? " << std::endl;
        std::cout << "2.Do you want to start a new game? " << std::endl;
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
        switch (choice) {
            case '1': {
                std::string filename;
                std::cout << "Enter the filename to load: ";
                std::cin >> filename;
                loadGame(filename + ".txt");
                displayBoard();
                PlayerVsPlayer();
                break;
            }
            case '2': {
                std::cout << "1.Do you want to start a game Player vs Player " << std::endl;
                std::cout << "2.Do you want to start a game Player vs Computer " << std::endl;
                std::cout << "Enter your choice: " << std::endl;
                std::cin >> input;
                if(input == 1){
                    displayBoard();
                    PlayerVsPlayer();
                }else if(input == 2){
                    PlayGameAI();
                }else{
                    std::cout<<"Invalid input"<<std::endl;
                }
                displayBoard();
                PlayerVsPlayer();
                break;
            }
            default:
                std::cout << "Invalid choice. Please choose either 1 or 2." << std::endl;
                break;
        }
    } while (choice != 1 && choice != 2); // Powtarzaj aż użytkownik wybierze poprawną opcję

}

void ChessGame::PlayerVsPlayer(){

    while (true) {
        // Wykonaj ruch dla bieżącego gracza
        std::cout << (isWhiteTurn ? "White's turn: " : "Black's turn: ");
        makeMove();

        // Przełącz turę
        isWhiteTurn = !isWhiteTurn;
    }
}
void ChessGame::PlayGameAI(){
    std::string from, to;
    bool gameOver = false;
    bool playerTurn = true; // Dla prostoty zaczyna gracz
    std::string input;
    displayBoard();
    while (!gameOver) {
        if (playerTurn) {
            std::cout<< "White's turn: " << "Enter move (e.g., A2 A4), 'CASTLE' for castling, or type 'SAVE' to save the game: ";
            std::cin >> input;
            std::transform(input.begin(), input.end(), input.begin(),
                           [](unsigned char c){ return std::toupper(c); });
            if (input == "SAVE") { // Obsługa przypadku, gdy gracz chce zapisać grę
                std::cout << "Enter filename to save ";
                std::string filename;
                std::cin >> filename;
                saveMovesToFile(filename+"_moves.txt");
                saveToFile(filename+".txt");
                exit(0);
            } else if (input == "CASTLE") { //Obsługa przypadku, gdy gracz chce wykonac roszadę
                std::string direction;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter 'KINGSIDE' or 'QUEENSIDE' for castling direction: ";
                std::cin >> direction;
                std::transform(direction.begin(), direction.end(), direction.begin(),
                               [](unsigned char c){ return std::toupper(c); });
                bool kingside = (direction == "KINGSIDE");
                castle(!playerTurn, kingside);
                displayBoard();
                break;
            }
            from = input;
            std::cin >> to;
            // Zamiana inputu na Uppercase
            std::transform(from.begin(), from.end(), from.begin(), ::toupper);
            std::transform(to.begin(), to.end(), to.begin(), ::toupper);
            movePiece(from,to);
            displayBoard();
        } else {
            // Wykonanie ruchu AI
            MoveAI aiMove = findBestMove(playerTurn);
            from = toChessNotation(aiMove.fromX,aiMove.fromY);
            to = toChessNotation(aiMove.toX,aiMove.toY);
            movePiece(from,to);
            displayBoard();
        }

        // Sprawdzenie warunków końca gry
        if (isCheckmate(!playerTurn)) { // Jeśli spełniony jest warunek szach-mat, zakończ grę
            gameOver = true;
        } else if (isStalemate(!playerTurn)) { // Jeśli spełniony jest warunek patu, zakończ grę
            gameOver = true;
        } else if (isDraw()) { // Jeśli spełniony jest warunek remisu, zakończ grę
            gameOver = true;
        } else if (isCheck(playerTurn)) { // Jeśli spełniony jest warunek szacha, wyświetl komunikat o szachu
            std::cout << "Check!" << std::endl;
        }
        // Przełącz tura gracza
        playerTurn = !playerTurn;
    }
}


bool ChessGame::isValidMove(const std::string& from, const std::string& to) {
    // Sprawdzenie poprawności formatu wejściowego
    if (!isValidInput(from, to)) {
        std::cout << "Invalid input format." << std::endl;
        return false;
    }
    // Konwersja notacji szachowej na indeksy tablicy
    int fromX = 8 - (from[1] - '0');
    int fromY = from[0] - 'A';
    int toX = 8 - (to[1] - '0');
    int toY = to[0] - 'A';


    // Upewnienie się, że współrzędne są w granicach planszy
    if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 || toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
        std::cout << "Coordinates out of bounds." << std::endl;
        return false;
    }

    // Upewnienie się, że na pozycji startowej znajduje się figura
    if (board[fromX][fromY] == nullptr) {
        std::cout << "No piece at the starting position." << std::endl;
        return false;
    }

    // Upewnienie się, że figura należy do aktualnego gracza
    if (isWhiteTurn && board[fromX][fromY]->getColor() != Piece::WHITE) {
        std::cout << "Wrong player's piece." << std::endl;
        return false;
    } else if (!isWhiteTurn && board[fromX][fromY]->getColor() != Piece::BLACK) {
        std::cout << "Wrong player's piece." << std::endl;
        return false;
    }

    // Wywołanie funkcji isValidMove dla konkretnej figury
    return board[fromX][fromY]->isValidMove(fromX, fromY, toX, toY, board);
}




void ChessGame::movePiece(const std::string& from, const std::string& to) {
    // Konwersja notacji szachowej na indeksy tablicy
    int fromRow = 8 - (from[1] - '0');
    int fromCol = from[0] - 'A';
    int toRow = 8 - (to[1] - '0');
    int toCol = to[0] - 'A';
    // Sprawdzenie czy figura istnieje na pozycji źródłowej
    Piece* piece = board[fromRow][fromCol];
    if (!piece) {
        std::cerr << "Error: No piece at the source position." << std::endl;
        return;
    }

    // Sprawdzenie czy jest możliwe bicie w przelocie i jeśli tak to wykonanie bicia
    if (isValidEnPassant(from, to)) {
        enPassantCapture(from, to);
        return;
    }

    // Przeniesienie figury na nową pozycję
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;
    // Sprawdzenie czy pionek doszedł do końca planszy i jeśli tak, wymień go na inną figurę
    if (piece->getType() == Piece::PAWN && (toRow == 0 || toRow == 7)) {
        promotePawn(to);
    }


    std::cout << "Moved " << piece->getTypeForBoard() << " from " << from << " to " << to << "." << std::endl;
    // Przełączenie tury
    isWhiteTurn = !isWhiteTurn;
}

void ChessGame::makeMove() {
    std::string from, to;

    // Pętla do-while wykonująca się dopóki gracz nie wprowadzi poprawnego ruchu lub nie wybierze opcji zapisu gry
    while (true) {
        // Wybór zapisu gry lub wykonania roszady lub ruchu
        std::cout << "Enter move (e.g., A2 A4), 'CASTLE' for castling, or type 'SAVE' to save the game: ";
        std::string input;
        std::cin >> input;

        std::transform(input.begin(), input.end(), input.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        if (input == "SAVE") { // Zapis gry do pliku po wybraniu opcji zapisu
            std::cout << "Enter filename to save ";
            std::string filename;
            std::cin >> filename;
            saveMovesToFile(filename+"_moves.txt");
            saveToFile(filename+".txt");
            exit(0); // Wyjście z pętli po wykonaniu zapisu
        } else if (input == "CASTLE") { // Sprawdzenie czy gracz chce wykonać roszadę
            std::string direction;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter 'KINGSIDE' or 'QUEENSIDE' for castling direction: ";
            std::cin >> direction;
            std::transform(direction.begin(), direction.end(), direction.begin(),
                           [](unsigned char c){ return std::toupper(c); });
            bool kingside = (direction == "KINGSIDE");
            castle(!isWhiteTurn, kingside);
            displayBoard();
            break; // Wykonanie roszady i wyjście z pętli po jej wykonaniu
        }

        from = input;
        std::cin >> to;

        // Konwersja ruchu na duże litery
        std::transform(from.begin(), from.end(), from.begin(), ::toupper);
        std::transform(to.begin(), to.end(), to.begin(), ::toupper);

        // Sprawdzenie poprawności ruchu
        if (!isValidMove(from, to)) {
            std::cout << "Invalid move. Try again." << std::endl;
            continue; // Restart the loop if the move is invalid
        }

        int fromRow = 8 - (from[1] - '0');
        int fromCol = from[0] - 'A';
        int toRow = 8 - (to[1] - '0');
        int toCol = to[0] - 'A';

        // Sprawdzenie czy król pozostanie w szchu
        Piece* piece = board[fromRow][fromCol];
        Piece* capturedPiece = board[toRow][toCol];
        board[toRow][toCol] = piece;
        board[fromRow][fromCol] = nullptr;

        bool stillInCheck = isCheck(!isWhiteTurn);

        // Undo ruch
        board[fromRow][fromCol] = piece;
        board[toRow][toCol] = capturedPiece;

        if (stillInCheck) {
            std::cout << "Move leaves king in check. Invalid move." << std::endl;
            continue; // Restart petli jesli ruch nic nie pomaga z szachem
        }


        // Sprawdzenie czy gracz wykonał ruch en passant
        if (isValidEnPassant(from, to)) {
            // Wykonanie ruchu en passant i wyjście z pętli po jego wykonaniu
            enPassantCapture(from, to);
            moveHistory.push_back({from, to}); // Dodanie ruchu do histori ruchów
            displayBoard();
            break;
        }
        // Wykonanie ruchu
        movePiece(from, to);
        // Dodanie ruchu do historii
        moveHistory.push_back({from, to});

        if (isCheckmate(!isWhiteTurn)) { // Sprawdzenie czy doszło do szach-matu
            std::cout << "Checkmate! " << (isWhiteTurn ? "Black wins!" : "White wins!") << std::endl;
            std::cout << "Do you want to save this game? Enter 'SAVE' ";
            std::string end;
            std::cin >> input;
            std::transform(input.begin(), input.end(), input.begin(),
                           [](unsigned char c){ return std::toupper(c); });
            if (input == "SAVE") {
                std::cout << "Enter filename to save ";
                std::string filename;
                std::cin >> filename;
                saveMovesToFile(filename + "_moves.txt");
                saveToFile(filename + ".txt");
            }else{
                std::cout<<"Invalid input"<<std::endl;
            }
            exit(0);
        } else if (isStalemate(!isWhiteTurn)) { // Sprawdzenie czy doszło do patu
            std::cout << "Stalmate!" << std::endl;
            std::cout << "Do you want to save this game? Enter 'SAVE' ";
            std::string end;
            std::cin >> input;
            std::transform(input.begin(), input.end(), input.begin(),
                           [](unsigned char c){ return std::toupper(c); });
            if (input == "SAVE") {
                std::cout << "Enter filename to save ";
                std::string filename;
                std::cin >> filename;
                saveMovesToFile(filename + "_moves.txt");
                saveToFile(filename + ".txt");
            }else{
                std::cout<<"Invalid input"<<std::endl;
            }
            exit(0);
        } else if (isDraw()) { // Sprawdzenie czy doszło do remisu
            std::cout << "Draw!" << std::endl;
            std::cout << "Do you want to save this game? Enter 'SAVE' ";
            std::string end;
            std::cin >> input;
            std::transform(input.begin(), input.end(), input.begin(),
                           [](unsigned char c){ return std::toupper(c); });
            if (input == "SAVE") {
                std::cout << "Enter filename to save ";
                std::string filename;
                std::cin >> filename;
                saveMovesToFile(filename + "_moves.txt");
                saveToFile(filename + ".txt");
            }else{
                std::cout<<"Invalid input"<<std::endl;
            }
            exit(0);
        } else if (isCheck(!isWhiteTurn)) { // Sprawdzenie czy doszło do szacha
            std::cout << "Check!" << std::endl;
        }

        // Zmiana tury
        isWhiteTurn = !isWhiteTurn;

        // Wyświetlenie zaktualizowanej planszy
        displayBoard();
        break;
    }
}


bool ChessGame::isValidInput(const std::string& from, const std::string& to) {
    // Sprawdzenie czy długość ciągów wejściowych wynosi 2
    if (from.length() != 2 || to.length() != 2) {
        return false;
    }

    // Check if the characters are within the range of 'A' to 'H' and '1' to '8'
    if (from[0] < 'A' || from[0] > 'H' || from[1] < '1' || from[1] > '8' ||
        to[0] < 'A' || to[0] > 'H' || to[1] < '1' || to[1] > '8') {
        return false;
    }

    return true;
}

// Wyświetlenie planszy szachowej na konsoli
void ChessGame::displayBoard() {
    std::cout << "    A    B    C    D    E    F    G    H" << std::endl;
    std::cout<<"  +----+----+----+----+----+----+----+----+"<<std::endl;// Wyświetlenie etykiet kolumn
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " |"; // Wyświetlenie numeru rzędu
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                std::cout <<" "<< board[i][j]->getTypeForBoard() << " |"; // Wyświetlenie typu figury
            } else {
                std::cout << "    |"; // Wyświetlenie pustego pola
            }
        }
        std::cout <<" "<<8 - i << std::endl; // Wyświetlenie numeru rzędu
        std::cout<<"  +----+----+----+----+----+----+----+----+"<<std::endl;
    }
    std::cout << "    A    B    C    D    E    F    G    H" << std::endl; // Wyświetlenie etykiet kolumn
}





bool ChessGame::isSquareAttacked(int x, int y, bool byWhite, std::vector<std::vector<bool>>& visited) {
    // Sprawdzenie poprawności współrzędnych pola
    if (!isValidSquare(x, y)) {
        std::cerr << "Error: Square (" << x << ", " << y << ") is out of bounds." << std::endl;
        return false;
    }

    // Jeśli pole zostało już odwiedzone, zwróć false, aby uniknąć nieskończonej rekursji
    if (visited[x][y]) {
        return false;
    }

    visited[x][y] = true;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->getColor() != (byWhite ? Piece::WHITE : Piece::BLACK)) {
                // Jeśli figura istnieje i należy do przeciwnego koloru, sprawdź czy może zaatakować pole (x, y)
                if (piece->isValidMove(i, j, x, y, board)) {
                    return true; //  Pole jest atakowane
                }
            }
        }
    }
    return false; // Jeśli żadna figura przeciwnika nie może zaatakować pola, zwróć false
}


bool ChessGame::isValidSquare(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool ChessGame::isCheckmate(bool white) {
    // Sprawdz szacha gracza
    if (!isCheck(white)) {
        // Gracz nie w szachu wiec nie mat
        return false;
    }

    // Przejdz przez pionki gracza
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->getColor() != (white ? Piece::WHITE : Piece::BLACK)) {
                // Przejdz przez wszystkie mozliwe ruchy dla danej figury
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        if (piece->isValidMove(i, j, x, y, board)) {
                            // Sumuluj ruch
                            Piece* originalPiece = board[x][y];
                            board[x][y] = piece;
                            board[i][j] = nullptr;

                            // Sprawdz czy król dalej jest w szachu po ruchu
                            bool stillInCheck = isCheck(white);

                            // Odwróc ruch
                            board[i][j] = piece;
                            board[x][y] = originalPiece;

                            if (!stillInCheck) {
                                // Jest ruch który wyprowadza króla z mata
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    // Brak ruchów i król jest dalej w szachu czyli szach mat
    return true;
}






bool ChessGame::isStalemate(bool white) {
    // Przejdz przez wszystkie figury
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->getColor() != (white ? Piece::WHITE : Piece::BLACK)) {
                // Przejdz przez wszystkie mozliwe ruchy dla danej figury
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        if (piece->isValidMove(i, j, x, y, board)) {
                            // Symuluj ruch
                            Piece* originalPiece = board[x][y];
                            board[x][y] = piece;
                            board[i][j] = nullptr;

                            // Znajdz pozycje króla przeciwnika po ruchu
                            int kingX = -1;
                            int kingY = -1;
                            for (int a = 0; a < 8; ++a) {
                                for (int b = 0; b < 8; ++b) {
                                    Piece* tempPiece = board[a][b];
                                    if (tempPiece != nullptr && tempPiece->getType() == Piece::KING && tempPiece->getColor() != (white ? Piece::WHITE : Piece::BLACK)) {
                                        kingX = a;
                                        kingY = b;
                                        break;
                                    }
                                }
                                if (kingX != -1) break;
                            }

                            if (kingX == -1 || kingY == -1) {
                                // Brak króla, bład
                                std::cerr << "Error: King not found on the board.1" << std::endl;
                                // Odwróć ruch
                                board[i][j] = piece;
                                board[x][y] = originalPiece;
                                continue;
                            }

                            // Sprawdz czy król dalej jest w szachu po ruchu
                            std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));
                            bool inCheck = isSquareAttacked(kingX, kingY, !white, visited);

                            // Odwróć ruch
                            board[i][j] = piece;
                            board[x][y] = originalPiece;

                            if (!inCheck) {
                                // Jest ruch który nie wprowadza króla do szacha
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    // Brak ruchów dla jakiegokolwiek pionka , pat
    return true;
}


bool ChessGame::isCheck(bool white) {
    // Znajdz króla gracza
    int kingX = -1;
    int kingY = -1;

    // Znajdz króla na planszy
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece *piece = board[i][j];
            if (piece != nullptr && piece->getType() == Piece::KING &&
                piece->getColor() != (white ? Piece::WHITE : Piece::BLACK)) {
                kingX = i;
                kingY = j;
                break;
            }
        }
        if (kingX != -1) break;
    }

    if (kingX == -1 || kingY == -1) {
        // Brak króla, bład
        std::cerr << "Error: King not found on the board.2" << std::endl;
        return false;
    }
    // Sprawdz czy król jest atakowany
    std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));
    bool isUnderCheck = isSquareAttacked(kingX, kingY, !white, visited);
    return isUnderCheck;
}



bool ChessGame::isDraw() {
    // Sprawdz stalmate
    if (isStalemate(true) || isStalemate(false)) {
        std::cout << "Stalemate!" << std::endl;
        return true;
    }

    // Sprawdz dla insufficient material
    if (isInsufficientMaterial()) {
        std::cout << "Draw by insufficient material!" << std::endl;
        return true;
    }
    return false;
}

bool ChessGame::isInsufficientMaterial() {
    int totalWhitePieces = 0;
    int totalBlackPieces = 0;

    // Zlicz pozostałe pionki dla kazdego gracza
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board[i][j];
            if (piece != nullptr) {
                if (piece->getColor() == Piece::WHITE) {
                    totalWhitePieces++;
                } else {
                    totalBlackPieces++;
                }
            }
        }
    }

    // Sprawdz insufficient material scenarios
    if (totalWhitePieces == 1 && totalBlackPieces == 1) {
        // Zostały tylko króle, remis
        return true;
    }

    return false;
}


void ChessGame::castle(bool isWhite, bool kingside) {
    int rank = isWhite ? 7 : 0;
    int kingStart = 4;
    int kingEnd = kingside ? 6 : 2;
    int rookStart = kingside ? 7 : 0;
    int rookEnd = kingside ? 5 : 3;

    // Sprawdź czy król i wieża są na swoich miejscach
    if (!isValidSquare(rank, kingStart) || !isValidSquare(rank, rookStart)) {
        std::cout << "Invalid castle move: King or rook is not in starting position." << std::endl;
        return;
    }

    Piece* king = board[rank][kingStart];
    Piece* rook = board[rank][rookStart];

    // Sprawdź czy na drodze króla nie ma żadnych przeszkód
    for (int i = kingStart + 1; i < kingEnd; ++i) {
        if (board[rank][i] != nullptr) {
            std::cout << "Invalid castle move: Path is obstructed." << std::endl;
            return;
        }
    }
    for (int i = kingStart - 1; i > kingEnd; ++i) {
        if (board[rank][i] != nullptr) {
            std::cout << "Invalid castle move: Path is obstructed." << std::endl;
            return;
        }
    }

    // Sprawdź czy król nie jest w szachu
    std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));
    if (isSquareAttacked(rank, kingStart, isWhite, visited)) {
        std::cout << "Invalid castle move: King is in check." << std::endl;
        return;
    }

    // Wykonaj ruch
    board[rank][kingEnd] = king;
    board[rank][kingStart] = nullptr;
    board[rank][rookEnd] = rook;
    board[rank][rookStart] = nullptr;

    // Dodaj ruch do historii
    moveHistory.push_back({toChessNotation(kingStart, rank), toChessNotation(kingEnd, rank)});
    moveHistory.push_back({toChessNotation(rookStart, rank), toChessNotation(rookEnd, rank)});

    std::cout << "Castle move successful." << std::endl;
}



void ChessGame::saveMovesToFile(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    // Zapis histori ruchów
    for (const auto& move : moveHistory) {
        outFile << move.from << " " << move.to;
        outFile << std::endl;
    }

    outFile.close();

    std::cout << "Game Moves saved to " << filename << std::endl;
}

void ChessGame::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    // Zapis stanu planszy
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (board[rank][file] == nullptr) {
                outFile << ". ";
            } else {
                outFile << board[rank][file]->getTypeForBoard() << " ";
            }
        }
        outFile << std::endl;
    }

    // Zapis tury
    outFile << (isWhiteTurn ? "WHITE" : "BLACK") << std::endl;

    outFile.close();

    std::cout << "Game saved to " << filename << std::endl;
}


void ChessGame::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }

    // Reset tablice i historie ruchów
    resetBoard();
    moveHistory.clear();

    std::string line;
    for (int rank = 0; rank < 8; rank++) {
        std::getline(inFile, line);
        std::istringstream iss(line);
        std::string pieceStr;
        for (int file = 0; file < 8; ++file) {
            iss >> pieceStr;
            if (pieceStr == ".") {
                board[rank][file] = nullptr;
            } else {
                char pieceChar = pieceStr[1];
                char pieceColor = pieceStr[0];
                Piece::PieceColor color;
                if(pieceColor == 'W'){
                    color = Piece::PieceColor::WHITE;
                }else{
                    color = Piece::PieceColor::BLACK;
                }
                pieceChar = toupper(pieceChar);
                switch (pieceChar) {
                    case 'K': board[rank][file] = new King(color); break;
                    case 'Q': board[rank][file] = new Queen(color); break;
                    case 'R': board[rank][file] = new Rook(color); break;
                    case 'B': board[rank][file] = new Bishop(color); break;
                    case 'N': board[rank][file] = new Knight(color); break;
                    case 'P': board[rank][file] = new Pawn(color); break;
                }
            }
        }
    }

    // Load the current turn
    std::getline(inFile, line);
    isWhiteTurn = (line == "WHITE");

    inFile.close();

    std::cout << "Game loaded from " << filename << std::endl;
}

void ChessGame::resetBoard() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (board[rank][file] != nullptr) {
                delete board[rank][file];
                board[rank][file] = nullptr;
            }
        }
    }
}


std::string ChessGame::toChessNotation(int x, int y) {
    char file = 'A' + y;  // Convert column index to file ('A' to 'H')
    int rank = 8 - x;     // Convert row index to rank (1 to 8)
    return std::string(1, file) + std::to_string(rank);
}



bool ChessGame::isValidEnPassant(const std::string& from, const std::string& to) {
    int fromRow = 8 - (from[1] - '0');
    int fromCol = from[0] - 'A';
    int toRow = 8 - (to[1] - '0');
    int toCol = to[0] - 'A';

    Piece* pawn = board[fromRow][fromCol];
    if (!pawn || pawn->getType() != Piece::PAWN) {
        return false;
    }

    // Check if the move is a valid en passant move
    int direction = isWhiteTurn ? 1 : -1;
    if (abs(fromCol - toCol) == 1 && toRow == fromRow + direction) {
        if (board[fromRow][toCol] != nullptr && board[fromRow][toCol]->getType() == Piece::PAWN &&
            board[fromRow][toCol]->getColor() != pawn->getColor()) {
            return true;
        }
    }

    return false;
}

void ChessGame::enPassantCapture(const std::string& from, const std::string& to) {
    int fromRow = 8 - (from[1] - '0');
    int fromCol = from[0] - 'A';
    int toRow = 8 - (to[1] - '0');
    int toCol = to[0] - 'A';

    Piece* pawn = board[fromRow][fromCol];
    if (!pawn || pawn->getType() != Piece::PAWN) {
        std::cerr << "Error: No pawn at the source position for en passant." << std::endl;
        return;
    }

    // Perform en passant capture
    board[toRow][toCol] = pawn;
    board[fromRow][fromCol] = nullptr;
    board[fromRow][toCol] = nullptr; // Remove the captured pawn

    std::cout << "En passant captured pawn from " << from << " to " << to << "." << std::endl;
    isWhiteTurn = !isWhiteTurn;
}
//Promocja Pionka
void ChessGame::promotePawn(const std::string& position) {
    int row = 8 - (position[1] - '0');
    int col = position[0] - 'A';

    Piece* piece = board[row][col];
    if (piece == nullptr || piece->getType() != Piece::PAWN) {
        std::cout << "No pawn to promote at " << position << std::endl;
        return;
    }

    std::cout << "Promoting pawn at " << position << std::endl;

    char choice;
    std::cout << "Promote pawn to (Q/R/B/N): ";
    std::cin >> choice;

    switch (choice) {
        case 'Q':
            board[row][col] = new Queen(piece->getColor());
            break;
        case 'R':
            board[row][col] = new Rook(piece->getColor());
            break;
        case 'B':
            board[row][col] = new Bishop(piece->getColor());
            break;
        case 'N':
            board[row][col] = new Knight(piece->getColor());
            break;
        default:
            std::cout << "Invalid choice. Pawn promoted to Queen by default." << std::endl;
            board[row][col] = new Queen(piece->getColor());
            break;
    }

    delete piece;
    std::cout << "Pawn promoted successfully!" << std::endl;
}

int ChessGame::evaluateBoard() {
    int score = 0;
    int pieceValues[6] = {100, 320, 330, 500, 900, 20000}; // Pawn, Knight, Bishop, Rook, Queen, King

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece != nullptr) {
                int value = pieceValues[piece->getType()];
                score += (piece->getColor() == Piece::WHITE) ? value : -value;
            }

        }
    }
    score += evaluatePawnStructure();
    return score;
}

int ChessGame::minimax( int depth, bool maximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard();
    }

    int bestScore;
    if (maximizingPlayer) {
        bestScore = std::numeric_limits<int>::min();
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                Piece* piece = board[i][j];
                if (piece != nullptr && piece->getColor() == Piece::BLACK) {
                    for (int k = 0; k < board.size(); ++k) {
                        for (int l = 0; l < board[k].size(); ++l) {
                            if (piece->isValidMove(i, j, k, l, board)) {
                                // Simulate move
                                Piece* capturedPiece = board[k][l];
                                board[k][l] = piece;
                                board[i][j] = nullptr;

                                bestScore = std::max(bestScore, minimax( depth - 1, false));

                                // Undo move
                                board[i][j] = piece;
                                board[k][l] = capturedPiece;
                            }
                        }
                    }
                }
            }
        }
    } else {
        bestScore = std::numeric_limits<int>::max();
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                Piece* piece = board[i][j];
                if (piece != nullptr && piece->getColor() == Piece::WHITE) {
                    for (int k = 0; k < board.size(); ++k) {
                        for (int l = 0; l < board[k].size(); ++l) {
                            if (piece->isValidMove(i, j, k, l, board)) {
                                // Simulate move
                                Piece* capturedPiece = board[k][l];
                                board[k][l] = piece;
                                board[i][j] = nullptr;

                                bestScore = std::min(bestScore, minimax( depth - 1, true));

                                // Undo move
                                board[i][j] = piece;
                                board[k][l] = capturedPiece;
                            }
                        }
                    }
                }
            }
        }
    }
    return bestScore;
}


ChessGame::MoveAI ChessGame::findBestMove(bool white) {
    int bestScore = std::numeric_limits<int>::min();
    MoveAI bestMove;
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->getColor() == Piece::BLACK) {
                for (int k = 0; k < board.size(); ++k) {
                    for (int l = 0; l < board[k].size(); ++l) {
                        if (piece->isValidMove(i, j, k, l, board)) {
                            // Simulate move
                            Piece* capturedPiece = board[k][l];
                            board[k][l] = piece;
                            board[i][j] = nullptr;

                            int moveScore = minimax( MAX_DEPTH, white);

                            // Undo move
                            board[i][j] = piece;
                            board[k][l] = capturedPiece;

                            if (moveScore > bestScore) {
                                bestScore = moveScore;
                                bestMove = {i, j, k, l};
                            }
                        }
                    }
                }
            }
        }
    }
    return bestMove;
}

int ChessGame::evaluatePawnStructure() {
    int score = 0;
    // Example: Penalize doubled pawns
    for (int col = 0; col < 8; col++) {
        int whitePawnsInCol = 0;
        int blackPawnsInCol = 0;
        for (int row = 0; row < 8; row++) {
            Piece* piece = board[row][col];
            if (piece != nullptr && piece->getType() == Piece::PAWN) {
                if (piece->getColor() == Piece::WHITE) {
                    whitePawnsInCol++;
                } else {
                    blackPawnsInCol++;
                }
            }
        }
        score -= (whitePawnsInCol > 1) ? 20 : 0; // Adjust the penalty as needed
        score += (blackPawnsInCol > 1) ? 20 : 0; // Adjust the penalty as needed
    }
    return score;
}

void ChessGame::printChessLogo() {
    std::cout << "_________  .__                             \n"
              << "\\_   ___ \\ |  |__    ____    ______  ______\n"
              << "/    \\  \\/ |  |  \\ _/ __ \\  /  ___/ /  ___/\n"
              << "\\     \\____|   Y  \\\\  ___/  \\___ \\  \\___ \\ \n"
              << " \\______  /|___|  / \\___  >/____  >/____  >\n"
              << "        \\/      \\/      \\/      \\/      \\/ \n";
}
