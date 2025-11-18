// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma  once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "display.h"
#include "game_types.h"
using namespace sf;
using namespace std;

class EventInputGame{// bat su kien trong game 
    private:
        WindowGame *m_window;
    public:
        TypeInPut typeInputMouse;
        EventInputGame();
        ~EventInputGame();
        void SetWindow(WindowGame *w);// set window vao event
        void CheckEvent();// kiem tra su kien la gi
};