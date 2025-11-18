// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma  once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace sf;
using namespace std;

class MouseGame{// class chuot
    private:
        Texture TextureMouse;// hinh
        Sprite PictureOfMouse;// hinh
        string locationImage;// dia chi
        Vector2i PosOfMouse;// position of mouse in screen
    public:
        MouseGame();// khoi tao
        ~MouseGame();// huy 
        void SetLocation(string lc);// lc la dia chi 
        void SetImage();//cai dat hinh 
        void GetPositionOfMouse(RenderWindow &wd);  // cap nhat vi tri cua chuot
        void DrawMouse(RenderWindow& wd);// ve chuot
        Vector2i GetPosOfMouse() const; // doc PosOfMouse tu ben ngoai
};