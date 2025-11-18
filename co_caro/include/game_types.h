#pragma once
#include <string>
#include <vector>
using namespace std;

// board.h
enum class PieceCaro{
    EMPTY,
    X,
    O
};

enum class WinnerCaro{
    NONE,
    X_WIN,
    O_WIN,
    DRAW
};

enum class BoardMode{
    NONE,
    MODE_3x3,
    MODE_5x5,
    MODE_10x10
};

// menu.h
enum class StateMenuCaro{
    WAIT,
    NEW_GAME,
    SELECT_MODE,
    INPUT_NAMES,
    SELECT_BOARD_SIZE,
    LOAD_GAME,
    LOAD_INPUT,
    ABOUT_GAME,
    EXIT_GAME
};

enum class ModeCaro{
    P1vsP2,
    PvsCPU,
    WAIT
};

struct VolumeCaro{
    int Sound;
    int Music;
};

// game.h
enum class GameState{
    PLAYING,
    SETTINGS,
    SAVING
};

enum class TurnPlayer{
    Player1,
    Player2_or_AI
};

enum class GameIconState{
    None,
    Setting,
    Undo,
    Redo,
    Restart,
    Quit
};

// file_manager.h
struct GameStateData{
    int boardSize;
    TurnPlayer nextTurn;
    vector <vector <PieceCaro>> boardState;
    string player1Name;
    int player1Wins;
    int player1Losses;
    string player2Name;
    int player2Wins;
    int player2Losses;
    BoardMode boardMode;
    ModeCaro gameMode;
};

// AIplayer.h
struct PatternScore{
    int fiveInRow = 0; // chuỗi 5 -> win
    int openFour = 0;  // chuỗi 4, 2 đầu k chặn
    int closedFour = 0; // như trên nhưng chặn 1 đầu
    int openThree = 0; // như trên nhưng 3
    int closedThree = 0; // cũng như trên
    int openTwo = 0;
    int potential = 0; // điểm tổng hợp
};

struct ScoreResult{ // Tổng hợp điểm từ 4 hướng ngang dọc 2 chéo + điểm từ patternscore
    int fiveInRow = 0;
    int openFour = 0;
    int closedFour = 0;
    int openThree = 0;
    int closedThree = 0;
    int openTwo = 0;
    int potential = 0;
};

// event_input.h
enum class TypeInPut{
	LEFT_MOUSE,   // nhấn chuột trái (tất cả input)
    WAIT_MOUSE    // chờ hoặc chưa nhấn chuột
};