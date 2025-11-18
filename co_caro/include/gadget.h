// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace sf;
using namespace std;

bool LoadTexture(Texture& tex, const string& path);
void TurnOffConsole();
void setupText(Text& text, const Font& font, int size, Color color);