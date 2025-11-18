// người code: Hoàng Tiến Dũng - B24DCCN152
#include "file_manager.h"

void FileManager::saveGame(
    const string& filename, const BoardCaro& board, TurnPlayer currentPlayer,
    ModeCaro currentGameMode, Player& p1, Player& p2
){
    ofstream outFile(filename);
    if (!outFile.is_open()){
        throw runtime_error("Error: Could not open file for saving: " + filename);
    }
    int size = board.GetBoardSize();
    outFile << size << "\n";
    outFile << static_cast<int>(board.GetModeBoard()) << "\n";
    outFile << static_cast<int>(currentGameMode) << "\n";
    outFile << (currentPlayer == TurnPlayer::Player1 ? 'X' : 'O') << "\n";
    outFile << p1.GetName() << "\n";
    outFile << p1.GetWins() << "\n";
    outFile << p1.GetLosses() << "\n";
    outFile << p2.GetName() << "\n";
    outFile << p2.GetWins() << "\n";
    outFile << p2.GetLosses() << "\n";
    const auto& grid = board.getGrid();
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            outFile << static_cast<int>(grid[i][j]) << (j == size - 1 ? "" : " ");
        }
        outFile << "\n";
    }
    outFile.close();
}

GameStateData FileManager::loadGame(const string& filename){
    ifstream inFile(filename);
    if (!inFile.is_open()){
        throw runtime_error("Error: Could not open file for loading: " + filename);
    }
    GameStateData loadedState;
    char turnChar;
    int cellValue;
    int tempMode;
    int tempGameMode;
    if (!(inFile >> loadedState.boardSize)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read board size.");
    }
    if (!(inFile >> tempMode)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read board mode.");
    }
    loadedState.boardMode = static_cast<BoardMode>(tempMode);
    if (!(inFile >> tempGameMode)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read game mode.");
    }
    loadedState.gameMode = static_cast<ModeCaro>(tempGameMode);
    if (!(inFile >> turnChar) || (turnChar != 'X' && turnChar != 'O')){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read or invalid turn character.");
    }
    loadedState.nextTurn = (turnChar == 'X') ? TurnPlayer::Player1 : TurnPlayer::Player2_or_AI;
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!getline(inFile, loadedState.player1Name)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 1 name.");
    }
    if (!(inFile >> loadedState.player1Wins)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 1 wins.");
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!(inFile >> loadedState.player1Losses)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 1 losses.");
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!getline(inFile, loadedState.player2Name)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 2 name.");
    }
    if (!(inFile >> loadedState.player2Wins)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 2 wins.");
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!(inFile >> loadedState.player2Losses)){
        inFile.close();
        throw runtime_error("Error: Invalid file format - could not read Player 2 losses.");
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    int size = loadedState.boardSize;
    loadedState.boardState.resize(size, vector<PieceCaro>(size));
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            if (!(inFile >> cellValue) || cellValue < 0 || cellValue > 2){
                inFile.close();
                stringstream ss;
                ss << "Error: Invalid file format - invalid or missing board data at row " << i + 1 << ", col " << j + 1;
                throw runtime_error(ss.str());
            }
            loadedState.boardState[i][j] = static_cast<PieceCaro>(cellValue);
        }
    }
    inFile.close();
    return loadedState;
}