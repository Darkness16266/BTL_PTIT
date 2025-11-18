// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class SoundGame{// class am thanh
    private:
        SoundBuffer buffer;
        unique_ptr<Sound> sound_game;
        string locationSound;// dia chi file am thanh
        bool repeatSound;// bien kiem tra lap lai
        int volumeSound;// am luong
    public:
        SoundGame();// khoi tao
        ~SoundGame();// huy am thanh
        bool SetSoundBuffer(const string& l);// l la dia chi file nhac
        void SetRepeatSound(bool repeat = true);// cai dat lap lai
        void Play();// chay
        void SetVolume(float vol);
};