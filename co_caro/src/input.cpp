// người code: Hoàng Tiến Dũng - B24DCCN152
#include "input.h"

MouseGame::MouseGame(): PictureOfMouse(TextureMouse){
    // Khởi tạo sprite ở tọa độ (0,0) ban đầu
    PictureOfMouse.setPosition({0.f, 0.f});
}

MouseGame::~MouseGame(){

}

void MouseGame::SetLocation(string lc){
	locationImage = lc;
}

void MouseGame::SetImage(){
	if(!TextureMouse.loadFromFile(locationImage)){
		cout<<"Loi mo file chuot\n";
	}
	else{
		PictureOfMouse.setTexture(TextureMouse, true);
		PictureOfMouse.setOrigin({8.f, 7.f});
	}
}

void MouseGame::GetPositionOfMouse(RenderWindow &wd){
	// Lấy tọa độ pixel của chuột thật
    PosOfMouse = Mouse::getPosition(wd);
    // Dịch sang tọa độ thế giới game
    Vector2f worldPos = wd.mapPixelToCoords(PosOfMouse);
    // Cập nhật vị trí của sprite con trỏ tùy chỉnh
    PictureOfMouse.setPosition(worldPos);
}

void MouseGame::DrawMouse(RenderWindow& wd){
	wd.draw(PictureOfMouse);
}

Vector2i MouseGame::GetPosOfMouse() const{
	return PosOfMouse;
}