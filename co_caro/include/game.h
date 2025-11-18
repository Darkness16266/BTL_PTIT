// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include "board.h"
#include "display.h"
#include "menu.h"
#include "music_game.h"
#include "input.h"
#include "sound.h"
#include "human_player.h"
#include "AIPlayer.h"
#include "event_input.h"
#include "game_types.h"
#include "file_manager.h"
#include "gadget.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <typeinfo>
#include <memory>
using namespace sf;
using namespace std;

class Game{
    private:
        // Các đối tượng chính
        WindowGame windowGame;
        BoardCaro board;
        MouseGame mouseGame;
        EventInputGame eventInput;

        // Biến logic
        GameState m_gameState;
        StateMenuCaro currentState;
        ModeCaro currentMode;
        TurnPlayer currentTurn;
        bool isGameOver;
        bool gameOverSoundPlayed;

        // Âm thanh
        MusicGame musicMenu, musicInGame;
        SoundGame soundClick, soundPlacePiece, soundWin, soundLose, soundNextTurn;
        float m_musicVolume, m_soundVolume;

        // Giao diện trong game
        Texture tex_gameBackground, tex_settingBackground, tex_savingBackground, tex_playerPanel, tex_resultPanel;
        Sprite spr_gameBackground, spr_settingBackground, spr_savingBackground, spr_playerPanel, spr_resultPanel;
        Texture tex_setting, tex_undo, tex_redo, tex_restart, tex_save, tex_quit;
        Sprite btn_setting, btn_undo, btn_redo, btn_restart, btn_save, btn_quit;

        Font font;
        // Panel P1
        Text m_p1InfoText;      // "Player 1(X)"
        Text m_p1NameText;      // Tên P1
        Text m_p1WinLossText;   // "Win: X / Lose: Y"
        // Panel P2
        Text m_p2InfoText;      // "Player 2(O)"
        Text m_p2NameText;      // Tên P2
        Text m_p2WinLossText;   // "Win: X / Lose: Y"
        // Kết quả
        Text m_resultText;     // "X WIN!", "O WIN!", "DRAW!"
        Text m_continueText;
        // Save
        Text m_savePromptText;     // Text "Nhap ten file can luu:"
        Text m_saveFileNameText;   // Text hiển thị tên file đang gõ
        string m_saveFileName;     // Chuỗi lưu tên file

        // Player
        unique_ptr <Player> p1; // Dùng con trỏ thông minh đến lớp cơ sở
        unique_ptr <Player> p2;
        Player* currentPlayerPtr; // Con trỏ đến người chơi hiện tại (p1 hoặc p2)
        bool isAIGame;

        // Các hàm private nội bộ
        void initInGameResources();                        // Tải tài nguyên cho màn hình chơi game
        void processInGameClick(const Vector2f& worldPos); // Xử lý click khi đang chơi
        void makeAIMove();                                 // Hàm để AI thực hiện nước đi
    public:
        Game();
        ~Game();
        bool ShouldReturnToMenu() const; // Hàm để main() kiểm tra
        void ResetForMenu();             // Hàm để reset game về trạng thái menu
        bool m_quitToMenu;               // Cờ báo hiệu muốn về menu
        void LoadImages(
            const string& a, const string& b, const string& c, const string& d,
            const string& e, const string& f, const string& g, const string& h,
            const string& i, const string& k, const string& l
        );
        
        // Các hàm điều khiển chính (để main.cpp gọi)
        void Init();            // Chạy menu và cài đặt ban đầu
        void Run();             // Chạy game
        bool IsRunning();       // Kiểm tra xem cửa sổ game có còn mở không
        void HandleInput();     // Xử lý tất cả sự kiện
        void Update();          // Cập nhật logic game (kiểm tra thắng/thua)
        void Render();          // Vẽ mọi thứ ra màn hình
};