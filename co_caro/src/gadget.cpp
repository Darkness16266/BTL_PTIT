#include "gadget.h"

bool LoadTexture(Texture& tex, const string& path){
    if (!tex.loadFromFile(path)){
        // cerr << "Load anh that bai: " << path << endl;
        return false;
    }
    else{
        // cout << "Load anh thanh cong: " << path << endl;
        return true;
    }
}

void TurnOffConsole(){
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}

void setupText(Text& text, const Font& font, int size, Color color){
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
}