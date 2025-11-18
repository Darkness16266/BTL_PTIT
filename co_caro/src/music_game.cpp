#include "music_game.h"

MusicGame::MusicGame(){
    repeatMusic = false;
    volumeMusic = 100.f;
}

MusicGame::~MusicGame(){
    musicGame.stop();
}

bool MusicGame::SetMusic(const string& l){
    if (!musicGame.openFromFile(l)){
        cerr << "Loi: Khong the mo file nhac: " << l << endl;
        return false;
    }
    musicGame.setVolume(volumeMusic); // Đặt âm lượng ban đầu
    musicGame.setLooping(repeatMusic); // Đặt lặp lại ban đầu
    return true;
}

void MusicGame::Play(){
    musicGame.play();
}

void MusicGame::Pause(){
    musicGame.pause();
}

void MusicGame::Stop(){
    musicGame.stop();
}

void MusicGame::SetRepeat(){
    repeatMusic=true;
    musicGame.setLooping(repeatMusic);
}

void MusicGame::SetVolume(float vol){
    volumeMusic = max(0.f, min(100.f, vol)); // Giới hạn 0-100
    musicGame.setVolume(volumeMusic);
}

SoundSource::Status MusicGame::GetStatus() const{
    return musicGame.getStatus();
}