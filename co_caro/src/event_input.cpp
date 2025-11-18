// người code: Hoàng Tiến Dũng - B24DCCN152
#include "event_input.h"

EventInputGame::EventInputGame(): m_window(nullptr), typeInputMouse(TypeInPut::WAIT_MOUSE){

}

EventInputGame::~EventInputGame(){

}

void EventInputGame::SetWindow(WindowGame *w){
	m_window = w;
	typeInputMouse = TypeInPut::WAIT_MOUSE;
}

void EventInputGame::CheckEvent(){
    typeInputMouse = TypeInPut::WAIT_MOUSE; // mặc định là chờ chuột
    RenderWindow* window = m_window->GetWindowGame();
    while (auto eventOpt = window->pollEvent()){
        Event event = *eventOpt;
        if (event.is<Event::Closed>()){
            m_window->Destroy();
        }
        else if (auto mouse = event.getIf<Event::MouseButtonPressed>()){
            if (mouse->button == Mouse::Button::Left){
                typeInputMouse = TypeInPut::LEFT_MOUSE;
            }
        }
    }
}