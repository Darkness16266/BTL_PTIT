#include "game.h"

int main(){
    TurnOffConsole();
    Game game;
    bool keepRunningApp = true;
    while (keepRunningApp){
        game.Init();
        if (game.IsRunning()){
            game.Run();
        }
        if (game.ShouldReturnToMenu()){
            game.ResetForMenu();
        }
        else{
            keepRunningApp = false;
        }
    }
}