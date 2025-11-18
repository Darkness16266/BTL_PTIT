// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>
using namespace sf;
using namespace std;

class WindowGame{
    private:
        RenderWindow m_window;// cua so game
        Vector2u m_windowSize;// kich thuong cua so
        string m_windowTitle;// ten cua so
        bool m_isFullScreen;// trang thai full man hinh
        bool m_isDone;// bien kiem tra het cua so window con hoat dong ko
    public:
        WindowGame();
        ~WindowGame();
        void Destroy();// tat cua so window
        void Create();// tao cua so window
        void SetUp(const string &title,const Vector2u &size);// set up ten cua so va size
        void BeginDraw();// bat dau ve
        void EndDraw();// ket thuc ve
        bool IsDone();// ket thuc chua
        bool IsFullScreen();// co full man hinh ko
        void Draw(const Drawable &d);// ve hinh
        Vector2u GetWindowSize();// tra ve size window game
        RenderWindow *GetWindowGame();// lay window
};