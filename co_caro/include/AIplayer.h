// người code: Đỗ Lê Trần Đạt - B24DCCN108
#pragma once
#include "player.h"
#include "game_types.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <climits>
#include <random>
#include <chrono>
using namespace std;

class AIplayer: public Player{
    private:
        int boardSize;
        int humanPlayer;
        
        bool isBoardEmpty(const vector<vector<int>>& board);
        pair<int, int> getBestMoveHeuristic(const vector<vector<int>>& board);
        int evaluatePosition(const vector<vector<int>>& board, int row, int col);
        int evaluateThreatInDirection(const vector<vector<int>>& board, int row, int col, int dx, int dy, int player);
        
        // best move cho từng size board
        pair<int, int> getBestMove3x3(const vector<vector<int>>& board);
        pair<int, int> getBestMove5x5(const vector<vector<int>>& board);
        pair<int, int> getBestMove10x10(const vector<vector<int>>& board);
        
        // phát hiện nước đi quyết định
        pair<int, int> findCriticalMove(const vector<vector<int>>& board);
        bool isBlockingFourInRow(const vector<vector<int>>& board, int row, int col);
        bool checkWin(const vector<vector<int>>& board, int player);
        
        // 10x10
        vector<pair<int, int>> getActiveArea(const vector<vector<int>>& board);
        int evaluatePosition10x10(const vector<vector<int>>& board, int row, int col);
        ScoreResult evaluateAllDirections(const vector<vector<int>>& board, int row, int col, int player);
        PatternScore evaluatePatternInDirection(const vector<vector<int>>& board, int row, int col, int dx, int dy, int player);
        int evaluatePositionValue10x10(int row, int col);
        
        // hàm random
        pair<int, int> getRandomOpeningMove();
        pair<int, int> getRandomGoodMove3x3(const vector<vector<int>>& board);
        pair<int, int> getRandomGoodMove10x10(const vector<vector<int>>& board);
        
    public:
        AIplayer(string n, PieceCaro p, int size);
        virtual ~AIplayer(); // Thêm destructor ảo
        virtual pair <int, int> GetMove(BoardCaro& board) override; // Override phương thức GetMove
};