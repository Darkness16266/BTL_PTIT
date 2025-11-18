// nguoi code: Hoang Tien Dung - B24DCCN152
#include "sound.h"

SoundGame::SoundGame(): repeatSound(false), volumeSound(100), sound_game(nullptr){

}

SoundGame::~SoundGame(){
    if (sound_game){
        sound_game->stop();
    }
}

bool SoundGame::SetSoundBuffer(const string& l){
    if (!buffer.loadFromFile(l)){
        cout << "Loi khong tim thay file nhac: " << l << endl;
        return false;
    }
    sound_game = make_unique<Sound>(buffer);
    return true;
}

void SoundGame::SetRepeatSound(bool repeat){
    repeatSound = repeat;
    if (sound_game){
        sound_game->setLooping(repeat);
    }
}

void SoundGame::Play(){
    if (sound_game){
        sound_game->play();
    }
}

void SoundGame::SetVolume(float vol){
    // Đảm bảo âm lượng nằm trong khoảng hợp lệ [0, 100]
    volumeSound = max(0.f, min(100.f, vol));
    if (sound_game){
        sound_game->setVolume(volumeSound); // Cập nhật âm lượng cho đối tượng sf::Sound
    }
}