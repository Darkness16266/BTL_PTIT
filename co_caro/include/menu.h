// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include "input.h"
#include "music_game.h"
#include "sound.h"
#include "display.h"
#include "event_input.h"
#include "file_manager.h"
#include "game_types.h"
#include "gadget.h"
using namespace sf;
using namespace std;

class MenuCaro{
    private:
        MouseGame mo_Menu;
        MusicGame mu_game;       // nhạc nền
        MusicGame in_game;       // nhạc ingame
        SoundGame so_click;      // âm thanh click chuột

        StateMenuCaro stateMenu;
        WindowGame m_window;
        EventInputGame e_window;
        ModeCaro modeGame;
        BoardCaro m_board;

        Texture tex_background, tex_background1, tex_background2;
        Sprite spr_background, spr_background1, spr_background2;
        Texture tex_newGame, tex_loadGame, tex_about, tex_exit; // giao diện menu chính
        Sprite btn_newGame, btn_loadGame, btn_about, btn_exit; // nút bấm
        Texture tex_aboutScreen;
        Sprite spr_aboutScreen;
        Texture tex_pvp, tex_pvc;
        Sprite btn_pvp, btn_pvc;
        Texture tex_3x3, tex_5x5, tex_10x10;
        Sprite btn_3x3, btn_5x5, btn_10x10;

        Font m_font;
        Text m_promptText1;       // Text "Nhập tên Player 1:"
        Text m_promptText2;       // Text "Nhập tên Player 2:"
        Text m_inputText1;        // Text mà P1 gõ vào
        Text m_inputText2;        // Text mà P2 gõ vào
        string m_player1Name;     // Chuỗi lưu tên P1
        string m_player2Name;     // Chuỗi lưu tên P2
        int m_currentPlayerInput; // 1 = nhập cho P1, 2 = nhập cho P2

        bool gameOverSoundPlayed;
        GameStateData m_loadedGameData;
        bool m_hasLoadedData;
        vector <string> m_saveFiles;
        Text m_saveFileListText;
        Text m_loadPromptText;
        Text m_loadInputText;
        string m_loadFileName;
        Text m_loadErrorText;
        void ScanSaveFiles();
        void UpdateSaveFileListText();
    public:
        MenuCaro();
        ~MenuCaro();
        void InitMouseMusicSound();
        void LoadImages(
            const string& a, const string& b, const string& c,
            const string& d, const string& e, const string& f,
            const string& g, const string& h, const string& i,
            const string& k, const string& l, const string& m, const string& n
        );
        string GetPlayer1Name() const;
        string GetPlayer2Name() const;
        void Update();
        void DrawMenu();
        void Run();
        ModeCaro GetModeGame();
        void HandleEvents();
        StateMenuCaro GetStateAfterRun() const;
        BoardMode selectedBoardMode;
        BoardMode GetBoardMode() const;
        GameStateData GetLoadedGameData() const;
};
