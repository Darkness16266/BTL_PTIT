// người code: Hoàng Tiến Dũng - B24DCCN152
#include "display.h"

WindowGame::WindowGame(){
	m_isFullScreen = false;
	m_isDone = false;
}

void WindowGame::SetUp(const string &title, const Vector2u &size){
	m_windowTitle=title;
	m_windowSize=size;
	m_isDone=false;
	Create();
}

WindowGame::~WindowGame(){
	Destroy();
}

void WindowGame::Create(){
	auto style = Style::Default;
    m_window.create(VideoMode({m_windowSize.x, m_windowSize.y}), m_windowTitle, style);
	// m_window.setFramerateLimit(60);
}

void WindowGame::Destroy(){
	m_window.close();
	m_isDone = true;
}

void WindowGame::BeginDraw(){
	m_window.clear(Color::Black);
}

void WindowGame::EndDraw(){
	m_window.display();
}

bool WindowGame::IsDone(){
	return m_isDone;
}

bool WindowGame::IsFullScreen(){
	return m_isFullScreen;
}

Vector2u WindowGame::GetWindowSize(){
	return m_windowSize;
}

void WindowGame::Draw(const Drawable &d){
	m_window.draw(d);
}

RenderWindow *WindowGame::GetWindowGame(){
	return &m_window;
}