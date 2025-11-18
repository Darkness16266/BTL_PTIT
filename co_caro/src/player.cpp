// người code: Hoàng Tiến Dũng - B24DCCN152
#include "player.h"

Player::Player(){
	Name = "Player";
	NumberOfWin = 0;
	NumberOfLose = 0;
    piece = PieceCaro::EMPTY;
}

Player::Player(string n, PieceCaro p){
	Name = n;
	NumberOfWin = 0;
    NumberOfLose = 0;
    piece = p;
}

Player::~Player(){

}

void Player::SetName(const string& newName){
    this->Name = newName;
}

string Player::GetName(){
    return this->Name;
}

void Player::UpdateNumberWinorLose(bool isWin){ 
    if (isWin){
        NumberOfWin++;
    }
	else{
        NumberOfLose++;
    }
}

int Player::GetWins() const{
    return this->NumberOfWin;
}

int Player::GetLosses() const{
    return this->NumberOfLose;
}

void Player::ResetScore(){
    NumberOfWin = 0;
    NumberOfLose = 0;
}

void Player::SetWins(int wins){
    if (wins >= 0){
        this->NumberOfWin = wins;
    }
}

void Player::SetLosses(int losses){
    if (losses >= 0){
        this->NumberOfLose = losses;
    }
}

PieceCaro Player::GetPiece() const{
    return this->piece;
}