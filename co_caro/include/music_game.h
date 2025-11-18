// người code: Hoàng Tiến Dũng - B24DCCN152
#pragma  once
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;

class MusicGame{
    private:
        Music musicGame;// nhac
        bool repeatMusic;// lap lai
        float volumeMusic;// am luong
    public:
        MusicGame();// khoi tao
        ~MusicGame();// huy
        bool SetMusic(const string& l);// l la dia chi file nhac
        void SetRepeat();// cai dat lap lai
        void Play();// chay
        void Pause();// dung
        void Stop();// tat
        void SetVolume(float vol);
        SoundSource::Status GetStatus() const;
};