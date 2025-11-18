// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include "player.h"
#include "display.h"
#include "board.h"
#include "event_input.h"

class HumanPlayer: public Player{
    private:
        EventInputGame& eventInput; // Tham chiếu đến đối tượng xử lý input
        WindowGame& windowGame;     // Tham chiếu đến WindowGame để lấy vị trí chuột
    public:
        // Constructor: nhận tên, quân cờ, tham chiếu EventInputGame và WindowGame
        HumanPlayer(string n, PieceCaro p, EventInputGame& input, WindowGame& window);
        // Destructor ảo
        virtual ~HumanPlayer();
        // Override phương thức GetMove
        virtual pair <int, int> GetMove(BoardCaro& board) override;
};