// Người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include "display.h"
#include "game_types.h"
#include "gadget.h"
using namespace sf;
using namespace std;

class BoardCaro{
    private:
        struct Move{
            int col;
            int row;
            PieceCaro piece;
        };
        stack <Move> undoStack; // Ngăn xếp lưu các nước đã đi
        stack <Move> redoStack; // Ngăn xếp lưu các nước đã undo
        
        Texture tmp;
        Texture tBoard3, tBoard5, tBoard10;        // ảnh nền bàn cờ
        Sprite sBoard;                             // sprite bàn cờ
        Texture tX3, tX5, tX10;                    // ảnh quân X
        Sprite sX;                                 // sprite quân X
        Texture tO3, tO5, tO10;                    // ảnh quân O
        Sprite sO;                                 // sprite quân O

        Vector2i lastMove;                         // để highlight nước đi cuối cùng
        int boardSize;                             // kích thước bàn cờ
        int cellSize;                              // kích thước mỗi ô (px)
        Vector2i posBoard;                         // vị trí vẽ bàn cờ trên màn hình
        vector <vector <PieceCaro>> board;         // trạng thái bàn cờ
        PieceCaro currentTurn;                     // lượt hiện tại (X hay O)
        WinnerCaro winner;                         // người thắng
        BoardMode mode;                            // chế độ bàn cờ
        int moveCount;                             // đếm số ô đã đánh để checkwin
    public:
        BoardCaro();
        ~BoardCaro();
        void LoadImages(
            const string& board3, const string& board5, const string& board10,
            const string& x3, const string& x5, const string& x10,
            const string& o3, const string& o5, const string& o10
        );
        void ScaleToWindow(WindowGame& windowGame);
        void SetModeBoard(BoardMode m);
        void DrawBoard(WindowGame* w);
        void DrawPieces(WindowGame* w);
        void Draw(WindowGame* w);
        bool PlacePiece(int c, int r);
        bool IsBoardFull() const;
        bool IsInsideBoard(int c, int r) const;
        PieceCaro GetPiece(int c, int r) const;
        WinnerCaro CheckWin(int c, int r) const;
        WinnerCaro GetWinner() const;
        PieceCaro GetCurrentTurn() const;
        void SwitchTurn();
        void ResetBoard();
        int GetBoardSize() const;
        Vector2i GetBoardPos(const Vector2f& mousePos) const;
        Vector2f getPosition() const;
        float getScaledSize() const;
        bool Undo(); // Hàm thực hiện Undo
        bool Redo(); // Hàm thực hiện Redo
        const vector <vector <PieceCaro>>& getGrid() const;
        BoardMode GetModeBoard() const;
        void setFromState(const GameStateData& state);
        vector <vector <int>> getBoardState() const;
};