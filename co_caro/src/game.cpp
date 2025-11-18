// người code: Hoàng Tiến Dũng - B24DCCN152
#include "game.h"

Game::Game():
    windowGame(),
    board(),
    musicMenu(),
    musicInGame(),
    soundClick(),
    soundPlacePiece(),
    soundWin(),
    soundLose(),
    soundNextTurn(),
    m_musicVolume(70.f),
    m_soundVolume(70.f),
    m_gameState(GameState::PLAYING),
    currentState(StateMenuCaro::WAIT),
    currentMode(ModeCaro::WAIT),
    currentTurn(TurnPlayer::Player1),
    isGameOver(false),
    gameOverSoundPlayed(false),
    spr_gameBackground(tex_gameBackground),
    btn_undo(tex_undo),
    btn_redo(tex_redo),
    btn_quit(tex_quit),
    btn_restart(tex_restart),
    btn_save(tex_save),
    btn_setting(tex_setting),
    spr_playerPanel(tex_playerPanel),
    spr_resultPanel(tex_resultPanel),
    spr_settingBackground(tex_settingBackground),
    spr_savingBackground(tex_savingBackground),
    m_p1InfoText(font),
    m_p1NameText(font),
    m_p1WinLossText(font),
    m_p2InfoText(font),
    m_p2NameText(font),
    m_p2WinLossText(font),
    m_resultText(font),
    m_continueText(font),
    m_savePromptText(font),
    m_saveFileNameText(font),
    m_quitToMenu(false),
    p1(nullptr), // Khởi tạo con trỏ là null
    p2(nullptr),
    currentPlayerPtr(nullptr),
    isAIGame(false),
    eventInput()
{
    eventInput.SetWindow(&windowGame); // Gán window cho eventInput
}

Game::~Game(){
    
}

void Game::Init(){
    musicInGame.Stop();
    MenuCaro menuGame;
    menuGame.Run();
    currentState = menuGame.GetStateAfterRun();
    currentMode = menuGame.GetModeGame();
    if (currentState == StateMenuCaro::NEW_GAME){
        // 1. Set up cửa sổ
        windowGame.SetUp("Caro Game", {1600, 900});
        windowGame.Create();
        windowGame.GetWindowGame()->setMouseCursorVisible(false);
        RenderWindow* window = windowGame.GetWindowGame();
        Texture tex_logo;
        Image logo("assets/images/logo.png");
        window->setIcon(logo);
        if (!tex_logo.resize(logo.getSize())){
            cerr << "ERROR!!!::COULD_NOT_RESIZE!" << endl;
        }
        tex_logo.update(logo);
        Sprite spr_logo(tex_logo);
        spr_logo.setTexture(tex_logo);
        // 2. Lấy mode đã chọn từ menu
        BoardMode selectedMode = menuGame.GetBoardMode();
        // 3. Set texture cho board
        this->board.SetModeBoard(selectedMode);
        // 4. Reset board
        board.ResetBoard();
        // 5. Scale
        board.ScaleToWindow(windowGame);

        string name1 = menuGame.GetPlayer1Name();
        string name2 = menuGame.GetPlayer2Name();
    }
    else if (currentState == StateMenuCaro::LOAD_GAME){
        // 1. Set up cửa sổ
        windowGame.SetUp("Caro Game", {1600, 900});
        windowGame.Create();
        windowGame.GetWindowGame()->setMouseCursorVisible(false);
        RenderWindow* window = windowGame.GetWindowGame();
        Texture tex_logo;
        Image logo("assets/images/logo.png");
        window->setIcon(logo);
        if (!tex_logo.resize(logo.getSize())){
            cerr << "ERROR!!!::COULD_NOT_RESIZE!" << endl;
        }
        tex_logo.update(logo);
        Sprite spr_logo(tex_logo);
        spr_logo.setTexture(tex_logo);
        // 2. Lấy dữ liệu đã được load từ menu
        GameStateData loadedData = menuGame.GetLoadedGameData();
        // 3. SET TEXTURE CHO BOARD (DỰA TRÊN DỮ LIỆU ĐÃ LOAD)
        this->board.SetModeBoard(loadedData.boardMode); 
        // 4. SCALE (Bây giờ đã an toàn)
        board.ScaleToWindow(windowGame);
        // 5. NẠP TRẠNG THÁI GAME VÀO BOARD
        board.setFromState(loadedData); 
        // 6. Cập nhật lượt chơi, tên người chơi...
        this->currentTurn = loadedData.nextTurn;
        this->currentMode = loadedData.gameMode;
    }
    else{
        windowGame.Destroy();
        m_quitToMenu = false;
        return;
    }
    initInGameResources();
    m_quitToMenu = false;
    // Khởi tạo người chơi
    if (currentState == StateMenuCaro::NEW_GAME){
        string name1 = menuGame.GetPlayer1Name();
        string name2 = menuGame.GetPlayer2Name();
        p1 = make_unique<HumanPlayer>(name1, PieceCaro::X, eventInput, windowGame);
        if (currentMode == ModeCaro::PvsCPU){
            p2 = make_unique<AIplayer>(name2, PieceCaro::O, board.GetBoardSize());
            isAIGame = true;
        }
        else{ // PvsP
            p2 = make_unique<HumanPlayer>(name2, PieceCaro::O, eventInput, windowGame);
            isAIGame = false;
        }
        // Điểm số mặc định là 0, 0 (được xử lý bởi constructor Player)
    } 
    else if (currentState == StateMenuCaro::LOAD_GAME){
        GameStateData loadedData = menuGame.GetLoadedGameData();
        p1 = make_unique<HumanPlayer>(loadedData.player1Name, PieceCaro::X, eventInput, windowGame);
        p1->SetWins(loadedData.player1Wins);
        p1->SetLosses(loadedData.player1Losses);
        if (currentMode == ModeCaro::PvsCPU){
            p2 = make_unique<AIplayer>(loadedData.player2Name, PieceCaro::O, board.GetBoardSize());
            p2->SetName(loadedData.player2Name);
            p2->SetWins(loadedData.player2Wins);
            p2->SetLosses(loadedData.player2Losses);
            isAIGame = true;
        }
        else{ // PvsP
            p2 = make_unique<HumanPlayer>(loadedData.player2Name, PieceCaro::O, eventInput, windowGame);
            p2->SetWins(loadedData.player2Wins);
            p2->SetLosses(loadedData.player2Losses);
            isAIGame = false;
        }
    }
    // Đặt người chơi hiện tại
    if (currentTurn == TurnPlayer::Player1){
        currentPlayerPtr = p1.get();
    }
    else{
        currentPlayerPtr = p2.get();
    }
    // Lấy thông tin vị trí của bàn cờ sau khi đã được căn lề trái
    Vector2f boardPos = board.getPosition();
    float boardSize = board.getScaledSize();

    // Các khoảng đệm
    float margin = 10.f;
    float spacing = 15.f; // Khoảng cách giữa các nút

    // 1. Co giãn các nút cho phù hợp
    float buttonHeight = 40.f;
    float buttonScale = buttonHeight / tex_undo.getSize().y;
    btn_undo.setScale({buttonScale, buttonScale});
    btn_redo.setScale({buttonScale, buttonScale});
    btn_restart.setScale({buttonScale, buttonScale});
    btn_quit.setScale({buttonScale, buttonScale});
    btn_setting.setScale({buttonScale, buttonScale});
    btn_save.setScale({buttonScale, buttonScale});

    // 2. Tính toán vị trí cho cột nút BÊN TRÁI
    // Lấy chiều rộng của một nút (giả sử chúng bằng nhau sau khi scale)
    float buttonWidth = btn_setting.getGlobalBounds().size.x;
    // Đặt cột nút ở bên trái bàn cờ, cách một khoảng margin
    float iconColumnX = boardPos.x - buttonWidth - margin + 8.f;

    // Vị trí Y: Bắt đầu từ đỉnh của bàn cờ (giữ nguyên)
    float currentY = boardPos.y;

    // 3. Đặt vị trí lần lượt cho từng nút (theo thứ tự từ trên xuống dưới)
    // Nút setting (trên cùng)
    btn_setting.setPosition({iconColumnX, currentY});
    currentY += btn_setting.getGlobalBounds().size.y + spacing;

    // Nút Undo
    btn_undo.setPosition({iconColumnX, currentY});
    currentY += btn_undo.getGlobalBounds().size.y + spacing;

    // Nút Redo
    btn_redo.setPosition({iconColumnX, currentY});
    currentY += btn_redo.getGlobalBounds().size.y + spacing;

    // Nút Restart
    btn_restart.setPosition({iconColumnX, currentY});
    currentY += btn_restart.getGlobalBounds().size.y + spacing;

    // Nút Save
    btn_save.setPosition({iconColumnX, currentY});
    currentY += btn_save.getGlobalBounds().size.y + spacing;

    // Nút Quit (dưới cùng)
    btn_quit.setPosition({iconColumnX, currentY});

    // ĐỊNH VỊ CÁC PANEL TRƯỚC TIÊN
    float margin1 = 50.f;
    float padding1 = 10.f; // Padding bên trong panel

    // Tọa độ X chung cho cả 2 panel bên phải
    float panelX = boardPos.x + boardSize + margin1;

    // ĐẶT VỊ TRÍ CHO PANEL 1 (spr_playerPanel)
    float panel1Y = boardPos.y; // Căn lề trên bằng bàn cờ
    spr_playerPanel.setPosition({panelX, panel1Y});

    // TÍNH TOÁN VỊ TRÍ X, Y CHO KHU VỰC PANEL 2 (không setPosition cho sprite thứ 2)
    float panel1Width = spr_playerPanel.getGlobalBounds().size.x; // Lấy width sau khi setPosition panel 1
    float panel2X = panelX + panel1Width + 5; // Vị trí X của panel 2
    float panel2Y = panel1Y;                   // <<< SỬA Ở ĐÂY: Y của panel 2 GIỐNG panel 1

    // ĐẶT VỊ TRÍ CHO PANEL DƯỚI (spr_resultPanel)
    float resultPanelHeight = spr_resultPanel.getGlobalBounds().size.y;
    float resultPanelY = boardPos.y + boardSize - resultPanelHeight; 
    spr_resultPanel.setPosition({panelX, resultPanelY});

    // 3. ĐỊNH VỊ TEXT DỰA VÀO VỊ TRÍ PANEL VỪA ĐẶT

    // Text Panel 1 (Dùng panelX, panel1Y)
    m_p1InfoText.setString("Player 1(X)");
    m_p1NameText.setString(p1->GetName());
    m_p1WinLossText.setString("Win: " + to_string(p1->GetWins()) + " / Lose: " + to_string(p1->GetLosses()));
    m_p1InfoText.setPosition({panelX + padding1, panel1Y + padding1});
    m_p1NameText.setPosition({panelX + padding1, panel1Y + padding1 + 50});
    m_p1WinLossText.setPosition({panelX + padding1, panel1Y + padding1 + 100});

    // Text Panel 2 (Dùng panelX, panel2Y)
    m_p2InfoText.setString("Player 2(O)");
    m_p2NameText.setString(p2->GetName());
    m_p2WinLossText.setString("Win: " + to_string(p2->GetWins()) + " / Lose: " + to_string(p2->GetLosses()));
    m_p2InfoText.setPosition({panel2X + padding1, panel2Y + padding1});
    m_p2NameText.setPosition({panel2X + padding1, panel2Y + padding1 + 50});
    m_p2WinLossText.setPosition({panel2X + padding1, panel2Y + padding1 + 100});

    // ĐỊNH VỊ TEXT KẾT QUẢ
    m_resultText.setString(""); // Ban đầu rỗng

    // 1. Lấy vị trí và kích thước của panel kết quả (panel dưới)
    Vector2f resultPanelPos = spr_resultPanel.getPosition();
    Vector2f resultPanelSize = spr_resultPanel.getGlobalBounds().size;

    // 2. Tính toán vị trí tâm X, Y của panel kết quả
    float resultCenterX = resultPanelPos.x + resultPanelSize.x / 2.f;
    float resultCenterY = resultPanelPos.y + resultPanelSize.y / 2.f;

    // 3. Căn giữa text kết quả (đặt origin trước setPosition)
    FloatRect textBounds = m_resultText.getLocalBounds();
    m_resultText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});

    // 4. Đặt text vào giữa panel kết quả
    m_resultText.setPosition({resultCenterX, resultCenterY});
    m_continueText.setString("Press 'Y' to continue or 'N' to go back to the menu");
    float continueY = panel1Y + resultPanelHeight + margin1 * 3 + 20.f;
    float continueX = panelX;
    m_continueText.setPosition({continueX, continueY});

    // 5. Đặt text phần lưu game
    float promptX = 460.f;
    float inputX  = 850.f;
    float textY   = 400.f;
    m_savePromptText.setString("Nhap ten file ban muon luu:");
    m_savePromptText.setOrigin({0.f, 0.f});
    m_savePromptText.setPosition({promptX, textY});
    m_saveFileNameText.setString("");
    m_saveFileNameText.setOrigin({0.f, 0.f});
    m_saveFileNameText.setPosition({inputX, textY});
}

bool Game::IsRunning(){
    return !windowGame.IsDone();
}

void Game::LoadImages(
    const string& a, const string& b, const string& c, const string& d,
    const string& e, const string& f, const string& g, const string& h,
    const string& i, const string& k, const string& l
){
    LoadTexture(tex_gameBackground, a);
    LoadTexture(tex_undo, b);
    LoadTexture(tex_redo, c);
    LoadTexture(tex_quit, d);
    LoadTexture(tex_setting, e);
    LoadTexture(tex_restart, f);
    LoadTexture(tex_save, g);
    LoadTexture(tex_playerPanel, h);
    LoadTexture(tex_resultPanel, i);
    LoadTexture(tex_settingBackground, k);
    LoadTexture(tex_savingBackground, l);
    
    tex_gameBackground.setSmooth(false);
    tex_undo.setSmooth(false);
    tex_redo.setSmooth(false);
    tex_quit.setSmooth(false);
    tex_setting.setSmooth(false);
    tex_restart.setSmooth(false);
    tex_save.setSmooth(false);
    tex_playerPanel.setSmooth(false);
    tex_resultPanel.setSmooth(false);
    tex_settingBackground.setSmooth(false);
    tex_savingBackground.setSmooth(false);

    spr_gameBackground.setTexture(tex_gameBackground, true);
    btn_undo.setTexture(tex_undo, true);
    btn_redo.setTexture(tex_redo, true);
    btn_quit.setTexture(tex_quit, true);
    btn_setting.setTexture(tex_setting, true);
    btn_restart.setTexture(tex_restart, true);
    btn_save.setTexture(tex_save, true);
    spr_playerPanel.setTexture(tex_playerPanel, true);
    spr_resultPanel.setTexture(tex_resultPanel, true);
    spr_settingBackground.setTexture(tex_settingBackground, true);
    spr_savingBackground.setTexture(tex_savingBackground, true);
}

void Game::initInGameResources(){
    mouseGame.SetLocation("assets/images/mouse.png");
    mouseGame.SetImage();

    LoadImages(
        "assets/images/game_background.png",
        "assets/images/undo_button.png",
        "assets/images/redo_button.png",
        "assets/images/quit_button.png",
        "assets/images/setting_button.png",
        "assets/images/restart_button.png",
        "assets/images/save_button.png",
        "assets/images/player_panel.png",
        "assets/images/result_panel.png",
        "assets/images/setting_background.png",
        "assets/images/background2.png"
    );

    musicInGame.SetMusic("assets/sound/ingame.mp3");
    musicInGame.SetRepeat();
    musicInGame.Play();

    soundPlacePiece.SetSoundBuffer("assets/sound/place.ogg");
    soundPlacePiece.SetRepeatSound(false);
    soundWin.SetSoundBuffer("assets/sound/win.ogg");
    soundWin.SetRepeatSound(false);
    soundLose.SetSoundBuffer("assets/sound/lose.ogg");
    soundLose.SetRepeatSound(false);
    soundNextTurn.SetSoundBuffer("assets/sound/turn.ogg");
    soundNextTurn.SetRepeatSound(false);
    soundClick.SetSoundBuffer("assets/sound/click.wav");
    soundClick.SetRepeatSound(false);

    if (!font.openFromFile("assets/fonts/arial.ttf")){
        cerr << "Không thể tải font: assets/fonts/arial.ttf" << endl;
    }

    // Panel P1 (Chữ đỏ)
    setupText(m_p1InfoText, font, 30, Color::Red);
    setupText(m_p1NameText, font, 30, Color::Red);
    setupText(m_p1WinLossText, font, 30, Color::Red);

    // Panel P2 (Chữ vàng)
    setupText(m_p2InfoText, font, 30, Color::Yellow);
    setupText(m_p2NameText, font, 30, Color::Yellow);
    setupText(m_p2WinLossText, font, 30, Color::Yellow);

    // Kết quả (Chữ xanh dương, to hơn)
    setupText(m_resultText, font, 50, Color::Blue);
    setupText(m_continueText, font, 30, Color::White);

    // Save
    setupText(m_savePromptText, font, 30, Color::White);
    setupText(m_saveFileNameText, font, 30, Color::Yellow);
}

void Game::HandleInput(){
    RenderWindow* window = windowGame.GetWindowGame();
    while (auto eventOpt = window->pollEvent()){
        Event event = *eventOpt;
        if (event.is<Event::Closed>()){
            windowGame.Destroy();
            m_quitToMenu = false;
        }
        if (auto key = event.getIf<Event::KeyPressed>()){
            if (m_gameState == GameState::SETTINGS){
                if (key->scancode == Keyboard::Scancode::Escape){
                    soundClick.Play();
                    m_gameState = GameState::PLAYING;
                }
            }
            if (m_gameState == GameState::SAVING){
                if (key->scancode == Keyboard::Scancode::Escape){
                    m_gameState = GameState::PLAYING;
                }
                else if (key->scancode == Keyboard::Scancode::Backspace && !m_saveFileName.empty()){
                    m_saveFileName.pop_back();
                    m_saveFileNameText.setString(m_saveFileName);
                }
                // Phím Enter: Xác nhận lưu
                else if (key->scancode == Keyboard::Scancode::Enter && !m_saveFileName.empty()){
                    string filenameToSave = m_saveFileName;
                    string extension = ".txt";
                    string saveFolderPath = "SaveGame/"; // <<< THƯ MỤC LƯU CỦA BẠN
                    // 1. Tự động thêm .txt nếu chưa có
                    // Kiểm tra xem 4 ký tự cuối có phải là ".txt" không (không phân biệt hoa thường)
                    if (filenameToSave.length() < 4 || filenameToSave.substr(filenameToSave.length() - 4) != ".txt"){
                        filenameToSave += extension;
                    }
                    // 2. Tạo đường dẫn đầy đủ
                    string fullPath = saveFolderPath + filenameToSave;
                    try{
                        // 3. Gọi hàm lưu game với đường dẫn đầy đủ
                        FileManager::saveGame(fullPath, board, currentTurn, currentMode, *p1, *p2);
                    }
                    catch (const filesystem::filesystem_error& e){
                        cerr << "Error creating directory: " << e.what() << endl;
                    }
                    catch (const runtime_error& e){
                        cerr << "Error saving game: " << e.what() << endl;
                    }
                    m_gameState = GameState::PLAYING;
                }
            }
            if (m_gameState == GameState::PLAYING || m_gameState == GameState::SETTINGS){
                if (key->scancode == Keyboard::Scancode::T){
                    soundClick.Play();
                    m_musicVolume = min(m_musicVolume + 10.f, 100.f);
                    musicInGame.SetVolume(m_musicVolume);
                    m_soundVolume = min(m_soundVolume + 10.f, 100.f);
                    soundClick.SetVolume(m_soundVolume);
                    soundPlacePiece.SetVolume(m_soundVolume);
                    soundWin.SetVolume(m_soundVolume);
                    soundLose.SetVolume(m_soundVolume);
                    soundNextTurn.SetVolume(m_soundVolume);
                }
                else if (key->scancode == Keyboard::Scancode::G){
                    soundClick.Play();
                    m_musicVolume = max(m_musicVolume - 10.f, 0.f);
                    musicInGame.SetVolume(m_musicVolume);
                    m_soundVolume = max(m_soundVolume - 10.f, 0.f);
                    soundClick.SetVolume(m_soundVolume);
                    soundPlacePiece.SetVolume(m_soundVolume);
                    soundWin.SetVolume(m_soundVolume);
                    soundLose.SetVolume(m_soundVolume);
                    soundNextTurn.SetVolume(m_soundVolume);
                }
                else if (key->scancode == Keyboard::Scancode::S){
                    soundClick.Play();
                    musicInGame.Stop();
                }
                else if (key->scancode == Keyboard::Scancode::Enter){
                    if (musicInGame.GetStatus() != SoundSource::Status::Playing){
                        soundClick.Play();
                        musicInGame.Play();
                    }
                }
                else if (key->scancode == Keyboard::Scancode::Space){
                    soundClick.Play();
                    if (musicInGame.GetStatus() == SoundSource::Status::Playing){
                        musicInGame.Pause();
                    }
                    else if (musicInGame.GetStatus() == SoundSource::Status::Paused){
                        musicInGame.Play();
                    }
                }
            }
            if (isGameOver){
                // Y: chơi tiếp
                if (key->scancode == Keyboard::Scancode::Y){
                    soundClick.Play();
                    board.ResetBoard(); 
                    currentTurn = TurnPlayer::Player1;
                    isGameOver = false;
                    gameOverSoundPlayed = false;
                    m_resultText.setString(""); 
                    m_p1WinLossText.setString("Win: " + to_string(p1->GetWins()) + " / Lose: " + to_string(p1->GetLosses()));
                    m_p2WinLossText.setString("Win: " + to_string(p2->GetWins()) + " / Lose: " + to_string(p2->GetLosses()));
                } 
                // N: về menu
                else if (key->scancode == Keyboard::Scancode::N){
                    soundClick.Play();
                    windowGame.Destroy();
                    m_quitToMenu = true; 
                }
            }
        }
        if (auto text = event.getIf<Event::TextEntered>()){
            if (m_gameState == GameState::SAVING){
                if (text->unicode < 128 && text->unicode != '\b' && text->unicode != '\r' && text->unicode != '\n'){
                    // Giới hạn độ dài tên file (ví dụ: 20 ký tự)
                    if (m_saveFileName.length() < 20) {
                        m_saveFileName += static_cast<char>(text->unicode);
                        m_saveFileNameText.setString(m_saveFileName);
                    }
                }
            }
        }
        if (auto mouse = event.getIf<Event::MouseButtonPressed>()){
            if (mouse->button == Mouse::Button::Left){
                Vector2f worldPos = window->mapPixelToCoords(mouse->position);
                if ((currentState == StateMenuCaro::NEW_GAME || currentState == StateMenuCaro::LOAD_GAME) && m_gameState == GameState::PLAYING && !isGameOver){
                    processInGameClick(worldPos);
                }
            }
        }
    }
}

void Game::processInGameClick(const Vector2f& worldPos){
    // Nếu game đã kết thúc, không xử lý click
    if (isGameOver){
        return;
    }
    
    // Nếu là lượt của AI và đang chơi với AI, không cho người chơi click
    if (isAIGame && currentTurn == TurnPlayer::Player2_or_AI){
        return;
    }
    
    Vector2i coords = board.GetBoardPos(worldPos);
    if (coords.x != -1){
        if (board.PlacePiece(coords.y, coords.x)){
            soundPlacePiece.Play();
            
            // Kiểm tra thắng/thua sau khi đánh
            if (board.GetWinner() != WinnerCaro::NONE){
                isGameOver = true;
                return;
            }
            // Nếu đang chơi với AI và chưa kết thúc
            if (isAIGame && board.GetWinner() == WinnerCaro::NONE){
                currentTurn = TurnPlayer::Player2_or_AI; // Chuyển sang lượt AI
                currentPlayerPtr = p2.get(); // CẬP NHẬT
                makeAIMove();
            }
            else{
                // Nếu không phải AI game, đổi lượt bình thường
                currentTurn = (currentTurn == TurnPlayer::Player1) ? TurnPlayer::Player2_or_AI : TurnPlayer::Player1;
                if (board.GetWinner() == WinnerCaro::NONE){
                    soundNextTurn.Play();
                }
            }
        }
    }
    else if (btn_setting.getGlobalBounds().contains(worldPos)){
        soundClick.Play();
        m_gameState = GameState::SETTINGS;
    }
    else if (btn_undo.getGlobalBounds().contains(worldPos) && !isAIGame){
        soundClick.Play();
        if (board.Undo()){
            // Nếu Undo thành công, đổi lại lượt chơi
            currentTurn = (currentTurn == TurnPlayer::Player1) ? TurnPlayer::Player2_or_AI : TurnPlayer::Player1;
            soundNextTurn.Play();
        }
    }
    else if (btn_redo.getGlobalBounds().contains(worldPos) && !isAIGame){
        soundClick.Play();
        if (board.Redo()){
            // Nếu Redo thành công, đổi lượt chơi
            currentTurn = (currentTurn == TurnPlayer::Player1) ? TurnPlayer::Player2_or_AI : TurnPlayer::Player1;
            soundNextTurn.Play();
        }
    }
    else if (btn_restart.getGlobalBounds().contains(worldPos)){
        soundClick.Play();
        board.ResetBoard();
        currentTurn = TurnPlayer::Player1;
        isGameOver = false;
        gameOverSoundPlayed = false;
    }
    else if (btn_save.getGlobalBounds().contains(worldPos)){
        soundClick.Play();
        m_gameState = GameState::SAVING; // <<< CHUYỂN TRẠNG THÁI
        m_saveFileName = ""; // Xóa tên file cũ (nếu có)
        m_saveFileNameText.setString(""); // Xóa text hiển thị
    }
    else if (btn_quit.getGlobalBounds().contains(worldPos)){
        soundClick.Play();
        windowGame.Destroy(); 
        m_quitToMenu = true;
    }
}

bool Game::ShouldReturnToMenu() const{
    return m_quitToMenu;
}

void Game::ResetForMenu() {
    // Reset các trạng thái của game, chuẩn bị cho lần chạy menu mới
    currentState = StateMenuCaro::WAIT;
    currentMode = ModeCaro::WAIT;
    currentTurn = TurnPlayer::Player1;
    isGameOver = false;
    gameOverSoundPlayed = false;
    m_quitToMenu = false;
    p1.reset(nullptr);
    p2.reset(nullptr);
    currentPlayerPtr = nullptr;
}

void Game::makeAIMove(){
    // 1. Kiểm tra p2 có phải là AI không
    AIplayer* aiPlayerPtr = dynamic_cast<AIplayer*>(p2.get());
    if (!isAIGame || !aiPlayerPtr || isGameOver){
        return;
    }
    cout << "Simple AI is thinking..." << endl;
    // // Lấy nước đi từ AI (aiPlayerPtr, tức là p2)
    pair<int, int> aiMove = aiPlayerPtr->GetMove(board);
    if (aiMove.first != -1 && aiMove.second != -1){
        cout << "AI decided to move: " << aiMove.first << "," << aiMove.second << endl;
        // AI đánh quân cờ - chú ý thứ tự tọa độ
        if (board.PlacePiece(aiMove.second, aiMove.first)){
            soundPlacePiece.Play();
            currentTurn = TurnPlayer::Player1;
            currentPlayerPtr = p1.get(); // CẬP NHẬT
            if (board.GetWinner() != WinnerCaro::NONE){
                isGameOver = true;
            }
        }
        else{
            cout << "AI move placement failed!" << endl;
            currentTurn = TurnPlayer::Player1;
            currentPlayerPtr = p1.get(); // CẬP NHẬT
        }
    }
    else{
        cout << "AI returned invalid move!" << endl;
        currentTurn = TurnPlayer::Player1;
        currentPlayerPtr = p1.get(); // CẬP NHẬT
    }
}

void Game::Update(){
    mouseGame.GetPositionOfMouse(*windowGame.GetWindowGame());
    if (currentState == StateMenuCaro::NEW_GAME && !gameOverSoundPlayed){
        WinnerCaro winner = board.GetWinner();
        if (winner != WinnerCaro::NONE) {
            if (winner == WinnerCaro::X_WIN || winner == WinnerCaro::O_WIN){
                soundWin.Play();
                if (winner == WinnerCaro::X_WIN){
                    p1->UpdateNumberWinorLose(true); // P1 (X) thắng
                    p2->UpdateNumberWinorLose(false); // P2 (O) thua
                    m_resultText.setString(p1->GetName() + " WIN!");
                }
                else if (winner == WinnerCaro::O_WIN){
                    p1->UpdateNumberWinorLose(false); // P1 (X) thua
                    p2->UpdateNumberWinorLose(true); // P2 (O) thắng
                    m_resultText.setString(p2->GetName() + " WIN!");
                }
            }
            else if (winner == WinnerCaro::DRAW){
                soundLose.Play();
                m_resultText.setString("DRAW!");
            }
            // Cập nhật lại text điểm số
            m_p1WinLossText.setString("Win: " + to_string(p1->GetWins()) + " / Lose: " + to_string(p1->GetLosses()));
            m_p2WinLossText.setString("Win: " + to_string(p2->GetWins()) + " / Lose: " + to_string(p2->GetLosses()));
            // Căn giữa lại text kết quả sau khi setString
            FloatRect textBounds = m_resultText.getLocalBounds();
            m_resultText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});
            isGameOver = true;
            gameOverSoundPlayed = true;
        }
    }
}

void Game::Render(){
    windowGame.BeginDraw();
    RenderWindow* window = windowGame.GetWindowGame();
    window->setView(window->getDefaultView());
    if (currentState == StateMenuCaro::NEW_GAME || currentState == StateMenuCaro::LOAD_GAME || isGameOver){
        window->draw(spr_gameBackground);
        board.Draw(&windowGame);
        window->draw(btn_setting);
        window->draw(btn_undo);
        window->draw(btn_redo);
        window->draw(btn_restart);
        window->draw(btn_save);
        window->draw(btn_quit);
        window->draw(spr_playerPanel);
        float panel1Width = spr_playerPanel.getGlobalBounds().size.x;
        float panel2X = spr_playerPanel.getPosition().x + panel1Width + 5;
        float panel2Y = spr_playerPanel.getPosition().y;
        Vector2f originalPos = spr_playerPanel.getPosition();
        window->draw(m_p1InfoText);
        window->draw(m_p1NameText);
        window->draw(m_p1WinLossText);
        spr_playerPanel.setPosition({panel2X, panel2Y});
        window->draw(spr_playerPanel);
        spr_playerPanel.setPosition(originalPos);
        window->draw(m_p2InfoText);
        window->draw(m_p2NameText);
        window->draw(m_p2WinLossText);
        window->draw(spr_resultPanel);
        if (isGameOver){
            window->draw(m_resultText);
            window->draw(m_continueText);
        }
    }
    if (m_gameState == GameState::SETTINGS){
        window->draw(spr_settingBackground);
    }
    else if (m_gameState == GameState::SAVING){
        window->draw(spr_savingBackground);
        window->draw(m_savePromptText);
        window->draw(m_saveFileNameText);
    }
    mouseGame.DrawMouse(*windowGame.GetWindowGame());
    windowGame.EndDraw();
}

void Game::Run(){
    while (IsRunning()){
        HandleInput();
        Update();
        Render();
    }
}