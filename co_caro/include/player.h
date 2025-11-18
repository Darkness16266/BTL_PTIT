// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "game_types.h"
#include "board.h"
using namespace std;

class Player{
    protected:
        string Name;        // Tên người chơi
        int NumberOfWin;    // Số trận thắng
        int NumberOfLose;   // Số trận thua
        PieceCaro piece;    // Thêm quân cờ mà người chơi này điều khiển (X hoặc O)
    public:
        Player();                               // Constructor mặc định
        Player(string n, PieceCaro p);          // Constructor có tham số
        ~Player();                              // Destructor
        string GetName();                       // Lấy tên
        void SetName(const string& newName);    // Đặt tên
        void UpdateNumberWinorLose(bool isWin); // Cập nhật số trận thắng/thua
        int GetWins() const;
        int GetLosses() const;
        void ResetScore();
        void SetWins(int wins);
        void SetLosses(int losses);
        PieceCaro GetPiece() const; // Hàm lấy quân cờ
        virtual pair<int, int> GetMove(BoardCaro& board) = 0; // = 0 biến lớp Player thành lớp trừu tượng
};