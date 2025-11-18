// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "board.h"
#include "game_types.h"
#include "player.h"
using namespace std;

class FileManager{
    public:
        static void saveGame(
            const string& filename, const BoardCaro& board, TurnPlayer currentPlayer,
            ModeCaro currentGameMode, Player& p1, Player& p2
        );
        static GameStateData loadGame(const string& filename);
};