// người code: Hoàng Tiến Dũng - B24DCCN152
#include "menu.h"

MenuCaro::MenuCaro():
    stateMenu(StateMenuCaro::WAIT), // Khởi tạo enum
    modeGame(ModeCaro::WAIT),       // Khởi tạo enum
    m_window(),                     // Gọi hàm khởi tạo mặc định của WindowGame
    m_board(),                      // Gọi hàm khởi tạo mặc định của BoardCaro
    e_window(),                     // Gọi hàm khởi tạo mặc định của EventInputGame
    mu_game(),
    so_click(),
    spr_background(tex_background),
    spr_background1(tex_background1),
    spr_background2(tex_background2),
    btn_newGame(tex_newGame),
    btn_loadGame(tex_loadGame),
    btn_about(tex_about),
    btn_exit(tex_exit),
    spr_aboutScreen(tex_aboutScreen),
    btn_pvp(tex_pvp),
    btn_pvc(tex_pvc),
    btn_3x3(tex_3x3),
    btn_5x5(tex_5x5),
    btn_10x10(tex_10x10),
    selectedBoardMode(BoardMode::NONE),
    m_promptText1(m_font),
    m_promptText2(m_font),
    m_inputText1(m_font),
    m_inputText2(m_font),
    m_player1Name(""),
    m_player2Name(""),
    m_currentPlayerInput(1),
    m_hasLoadedData(false),
    m_saveFileListText(m_font),
    m_loadPromptText(m_font),
    m_loadErrorText(m_font),
    m_loadInputText(m_font),
    m_loadFileName("")
{
    m_window.SetUp("Caro Game", {1600, 900});
    m_window.Create();
    m_window.GetWindowGame()->setMouseCursorVisible(false);
    RenderWindow* window = m_window.GetWindowGame();
    Texture tex_logo;
    Image logo("assets/images/logo.png");
    window->setIcon(logo);
    if (!tex_logo.resize(logo.getSize())){
        cerr << "ERROR!!!::COULD_NOT_RESIZE!" << endl;
    }
    tex_logo.update(logo);
    Sprite spr_logo(tex_logo);
    spr_logo.setTexture(tex_logo);
    LoadImages(
        "assets/images/background.png",
        "assets/images/new_game_button.png",
        "assets/images/load_game_button.png",
        "assets/images/about_button.png",
        "assets/images/exit_button.png",
        "assets/images/about.png",
        "assets/images/pvp_button.png",
        "assets/images/pvc_button.png",
        "assets/images/3x3_button.png",
        "assets/images/5x5_button.png",
        "assets/images/10x10_button.png",
        "assets/images/background1.png",
        "assets/images/background2.png"
    );
    InitMouseMusicSound();
    if (!m_font.openFromFile("assets/fonts/arial.ttf")){
        cerr << "Không thể tải font: assets/fonts/arial.ttf" << endl;
    }
    float p1_Y = 400.f;
    float p2_Y = 450.f;
    float prompt_X = 500.f;
    float input_X = 750.f;

    // Phần nhập tên
    // Cài đặt cho "Nhap ten Player 1:" (Màu đỏ)
    setupText(m_promptText1, m_font, 28, Color::Red);
    m_promptText1.setString("Nhap ten Player 1:");
    m_promptText1.setPosition({prompt_X, p1_Y});

    // Cài đặt cho text P1 nhập vào (Màu đỏ)
    setupText(m_inputText1, m_font, 28, Color::Red);
    m_inputText1.setPosition({input_X, p1_Y});

    // Cài đặt cho "Nhap ten Player 2:" (Màu xanh)
    setupText(m_promptText2, m_font, 28, Color::Blue);
    m_promptText2.setString("Nhap ten Player 2:");
    m_promptText2.setPosition({prompt_X, p2_Y});

    // Cài đặt cho text P2 nhập vào (Màu xanh)
    setupText(m_inputText2, m_font, 28, Color::Blue);
    m_inputText2.setPosition({input_X, p2_Y});

    // Phần load file
    float loadStartX = 460.f;
    float loadStartY = 400.f;

    setupText(m_saveFileListText, m_font, 28, Color::White);
    m_saveFileListText.setPosition({loadStartX, loadStartY});
    m_saveFileListText.setString("Danh sach file da luu:");

    setupText(m_loadPromptText, m_font, 28, Color::White);
    m_loadPromptText.setPosition({loadStartX, loadStartY + 150});
    m_loadPromptText.setString("Nhap ten file muon load:");

    setupText(m_loadInputText, m_font, 28, Color::Yellow);
    m_loadInputText.setPosition({loadStartX + m_loadPromptText.getGlobalBounds().size.x + 10.f, loadStartY + 150});
    m_loadInputText.setString("");

    setupText(m_loadErrorText, m_font, 28, Color::Red);
    m_loadErrorText.setPosition({loadStartX, loadStartY + 200});
    m_loadErrorText.setString("");
}

MenuCaro::~MenuCaro(){

}

// Khởi tạo chuột, nhạc, âm thanh
void MenuCaro::InitMouseMusicSound(){
    mo_Menu.SetLocation("assets/images/mouse.png");
    mo_Menu.SetImage();

    mu_game.SetMusic("assets/sound/menu.mp3");
    mu_game.SetRepeat();
    mu_game.Play();
    in_game.SetMusic("assets/sound/ingame.mp3");
    in_game.SetRepeat();

    so_click.SetSoundBuffer("assets/sound/click.wav");
    so_click.SetRepeatSound(false);
}

void MenuCaro::LoadImages(
    const string& a, const string& b, const string& c,
    const string& d, const string& e, const string& f,
    const string& g, const string& h, const string& i,
    const string& k, const string& l, const string& m, const string& n
){
    LoadTexture(tex_background, a);
    LoadTexture(tex_newGame, b);
    LoadTexture(tex_loadGame, c);
    LoadTexture(tex_about, d);
    LoadTexture(tex_exit, e);
    LoadTexture(tex_aboutScreen, f);
    LoadTexture(tex_pvp, g);
    LoadTexture(tex_pvc, h);
    LoadTexture(tex_3x3, i);
    LoadTexture(tex_5x5, k);
    LoadTexture(tex_10x10, l);
    LoadTexture(tex_background1, m);
    LoadTexture(tex_background2, n);

    tex_background.setSmooth(false);
    tex_newGame.setSmooth(false);
    tex_loadGame.setSmooth(false);
    tex_about.setSmooth(false);
    tex_exit.setSmooth(false);
    tex_aboutScreen.setSmooth(false);
    tex_pvp.setSmooth(false);
    tex_pvc.setSmooth(false);
    tex_3x3.setSmooth(false);
    tex_5x5.setSmooth(false);
    tex_10x10.setSmooth(false);
    tex_background1.setSmooth(false);
    tex_background2.setSmooth(false);

    spr_background.setTexture(tex_background, true);
    btn_newGame.setTexture(tex_newGame, true);
    btn_loadGame.setTexture(tex_loadGame, true);
    btn_about.setTexture(tex_about, true);
    btn_exit.setTexture(tex_exit, true);
    spr_aboutScreen.setTexture(tex_aboutScreen, true);
    btn_pvp.setTexture(tex_pvp, true);
    btn_pvc.setTexture(tex_pvc, true);
    btn_3x3.setTexture(tex_3x3, true);
    btn_5x5.setTexture(tex_5x5, true);
    btn_10x10.setTexture(tex_10x10, true);
    spr_background1.setTexture(tex_background1, true);
    spr_background2.setTexture(tex_background2, true);

    // 1. Lấy kích thước cửa sổ và ảnh nền
    Vector2u windowSize = m_window.GetWindowSize();
    Vector2u bgTextureSize = tex_background.getSize();
    
    // 2. Co giãn ảnh nền để vừa với cửa sổ
    float scaleX = static_cast<float>(windowSize.x) / bgTextureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / bgTextureSize.y;
    spr_background.setScale({scaleX, scaleY});
    spr_background.setPosition({0.f, 0.f});
    
    // Căn giữa màn hình about
    spr_aboutScreen.setScale({scaleX, scaleY});
    spr_aboutScreen.setPosition({0.f, 0.f});

    // LOGIC BỐ CỤC MỚI

    // 1. Tính toán tỉ lệ co giãn dựa trên CHIỀU CAO của cửa sổ.
    // SỬA LỖI: Giảm chiều cao mỗi nút xuống còn 9% để vừa vặn hơn.
    float buttonHeightAvailable = windowSize.y * 0.09f;
    float buttonScale = buttonHeightAvailable / tex_newGame.getSize().y;

    // Áp dụng tỉ lệ cho tất cả các nút
    btn_newGame.setScale({buttonScale, buttonScale});
    btn_loadGame.setScale({buttonScale, buttonScale});
    btn_about.setScale({buttonScale, buttonScale});
    btn_exit.setScale({buttonScale, buttonScale});

    btn_pvp.setScale({buttonScale, buttonScale});
    btn_pvc.setScale({buttonScale, buttonScale});

    btn_3x3.setScale({buttonScale, buttonScale});
    btn_5x5.setScale({buttonScale, buttonScale});
    btn_10x10.setScale({buttonScale, buttonScale});

    // 2. Tính toán vị trí để căn giữa toàn bộ khối nút
    float scaledButtonHeight = btn_newGame.getGlobalBounds().size.y;
    float spacing = scaledButtonHeight * 0.15f; // Khoảng cách bằng 15% chiều cao nút
    float centerX = windowSize.x / 2.0f;
    
    // 3. Hàm trợ giúp để căn giữa và đặt vị trí cho nút
    auto centerAndSet = [&](Sprite& button, float x, float y) {
        FloatRect bounds = button.getLocalBounds();
        button.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        button.setPosition({x, y});
    };
    
    // 4. Đặt vị trí cho các nút một cách có hệ thống
    float contentStartY = windowSize.y * 0.20f; // SỬA: Dịch khối nút lên cao hơn, bắt đầu từ 40% chiều cao màn hình
    float contentHeight = windowSize.y - contentStartY;

    // Căn giữa khối 4 nút vào khu vực đã định
    float totalHeight_4_buttons = (scaledButtonHeight * 4) + (spacing * 3);
    float startY_4_buttons = contentStartY + (contentHeight - totalHeight_4_buttons) / 2.0f;
    centerAndSet(btn_newGame, centerX, startY_4_buttons + scaledButtonHeight * 0.5f);
    centerAndSet(btn_loadGame, centerX, startY_4_buttons + scaledButtonHeight * 1.5f + spacing);
    centerAndSet(btn_about, centerX, startY_4_buttons + scaledButtonHeight * 2.5f + spacing * 2);
    centerAndSet(btn_exit, centerX, startY_4_buttons + scaledButtonHeight * 3.5f + spacing * 3);

    // Căn giữa khối 2 nút vào khu vực đã định
    float totalHeight_2_buttons = (scaledButtonHeight * 2) + spacing;
    float startY_2_buttons = contentStartY + (contentHeight - totalHeight_2_buttons) / 2.0f;
    centerAndSet(btn_pvp, centerX, startY_2_buttons + scaledButtonHeight * 0.5f);
    centerAndSet(btn_pvc, centerX, startY_2_buttons + scaledButtonHeight * 1.5f + spacing);

    // Căn giữa khối 3 nút vào khu vực đã định
    float totalHeight_3_buttons = (scaledButtonHeight * 3) + (spacing * 2);
    float startY_3_buttons = contentStartY + (contentHeight - totalHeight_3_buttons) / 2.0f;
    centerAndSet(btn_3x3, centerX, startY_3_buttons + scaledButtonHeight * 0.5f);
    centerAndSet(btn_5x5, centerX, startY_3_buttons + scaledButtonHeight * 1.5f + spacing);
    centerAndSet(btn_10x10, centerX, startY_3_buttons + scaledButtonHeight * 2.5f + spacing * 2);
}

// Lấy chế độ chơi
ModeCaro MenuCaro::GetModeGame(){
    return modeGame;
}

// Click chuột vào ô
void MenuCaro::HandleEvents(){
    cout << "Current State: " << static_cast<int>(stateMenu) << "\r";
    RenderWindow* window = m_window.GetWindowGame();
    while (auto eventOpt = window->pollEvent()){
        Event event = *eventOpt; // Lấy sự kiện ra từ optional
        // 1. Xử lý sự kiện đóng cửa sổ (luôn kiểm tra)
        if (event.is<Event::Closed>()){
            m_window.Destroy();
            stateMenu = StateMenuCaro::EXIT_GAME;
        }
        // 2. Xử lý sự kiện nhập Text
        if (auto text = event.getIf<Event::TextEntered>()){
            if (stateMenu == StateMenuCaro::INPUT_NAMES){
                if (text->unicode < 128 && text->unicode != '\b' && text->unicode != '\r' && text->unicode != '\n'){
                    if (m_currentPlayerInput == 1){
                        m_player1Name += static_cast<char>(text->unicode);
                        m_inputText1.setString(m_player1Name);
                    } 
                    // <<< THÊM ĐIỀU KIỆN KIỂM TRA PVP >>>
                    else if (m_currentPlayerInput == 2 && modeGame == ModeCaro::P1vsP2){
                        m_player2Name += static_cast<char>(text->unicode);
                        m_inputText2.setString(m_player2Name);
                    }
                }
            }
            else if (stateMenu == StateMenuCaro::LOAD_INPUT){
                // Lọc ký tự điều khiển, chỉ nhận ASCII in được
                if (text->unicode >= 32 && text->unicode < 127){
                    // Giới hạn độ dài tên file (20 ký tự)
                    if (m_loadFileName.length() < 20){
                        m_loadFileName += static_cast<char>(text->unicode);
                        m_loadInputText.setString(m_loadFileName);
                        m_loadErrorText.setString(""); // Xóa lỗi khi bắt đầu gõ lại
                    }
                }
            }
        }
        // 3. XỬ LÝ SỰ KIỆN NHẤN PHÍM
        if (auto key = event.getIf<Event::KeyPressed>()){
            // Nhấn ESC: Quay về menu chính
            if (key->scancode == Keyboard::Scancode::Escape && stateMenu != StateMenuCaro::WAIT){
                if (stateMenu == StateMenuCaro::NEW_GAME){
                    in_game.Stop();
                    mu_game.Play();
                }
                stateMenu = StateMenuCaro::WAIT;
            }
            else if (stateMenu == StateMenuCaro::INPUT_NAMES){
                // Xử lý phím BACKSPACE
                if (key->scancode == Keyboard::Scancode::Backspace){
                    if (m_currentPlayerInput == 1 && !m_player1Name.empty()){
                        m_player1Name.pop_back();
                        m_inputText1.setString(m_player1Name);
                    } 
                    // <<< THÊM ĐIỀU KIỆN KIỂM TRA PVP >>>
                    else if (m_currentPlayerInput == 2 && !m_player2Name.empty() && modeGame == ModeCaro::P1vsP2){
                        m_player2Name.pop_back();
                        m_inputText2.setString(m_player2Name);
                    }
                }
                // Xử lý phím ENTER
                else if (key->scancode == Keyboard::Scancode::Enter){
                    if (modeGame == ModeCaro::P1vsP2){
                        // Chế độ PVP: Enter lần 1 chuyển sang P2, Enter lần 2 chuyển sang chọn bàn cờ
                        if (m_currentPlayerInput == 1){
                            m_currentPlayerInput = 2; // Chuyển sang nhập cho P2
                        }
                        else{
                            // Đã nhập xong cho P2 -> Chuyển sang chọn bàn cờ
                            stateMenu = StateMenuCaro::SELECT_BOARD_SIZE;
                        }
                    }
                    else{
                        // Chế độ PVC: Chỉ cần nhập P1, Enter là chuyển sang chọn bàn cờ
                        stateMenu = StateMenuCaro::SELECT_BOARD_SIZE;
                    }
                }
            }
            else if (stateMenu == StateMenuCaro::LOAD_INPUT){
                // Phím ESC: Hủy load, quay về menu chính (WAIT)
                if (key->scancode == Keyboard::Scancode::Escape){
                    stateMenu = StateMenuCaro::WAIT;
                    m_loadErrorText.setString(""); // Xóa lỗi
                }
                // Phím Backspace: Xóa ký tự cuối
                else if (key->scancode == Keyboard::Scancode::Backspace && !m_loadFileName.empty()){
                    m_loadFileName.pop_back();
                    m_loadInputText.setString(m_loadFileName);
                    m_loadErrorText.setString(""); // Xóa lỗi
                }
                // Phím Enter: Xác nhận tên file và thử load
                else if ((key->scancode == Keyboard::Scancode::Enter) && !m_loadFileName.empty()){
                    string filenameToLoad = m_loadFileName;
                    string saveFolderPath = "SaveGame/";
                    string fullPath;
                    // Tự động thêm .txt
                    if (filenameToLoad.length() < 4 || filenameToLoad.substr(filenameToLoad.length() - 4) != ".txt"){
                        filenameToLoad += ".txt";
                    }
                    fullPath = saveFolderPath + filenameToLoad;
                    try{
                        m_loadedGameData = FileManager::loadGame(fullPath);
                        m_hasLoadedData = true;
                        stateMenu = StateMenuCaro::LOAD_GAME;
                        m_window.Destroy(); // Đóng menu
                        cout << "Load game successful from: " << fullPath << endl;
                    }
                    catch (const runtime_error& e){
                        cerr << "Error loading game: " << e.what() << endl;
                        m_loadErrorText.setString("Loi: " + string(e.what()));
                        m_hasLoadedData = false;
                    }
                }
            }
        }
        // 4. XỬ LÝ SỰ KIỆN NHẤN CHUỘT (code của bạn được đặt ở đây)
        if (auto mouse = event.getIf<Event::MouseButtonPressed>()){
            if (mouse->button == Mouse::Button::Left){
                Vector2i pixelPos = Mouse::getPosition(*window);
                Vector2f worldPos = window->mapPixelToCoords(pixelPos);
                // Dùng switch để xử lý click tùy theo trạng thái game
                switch (stateMenu){
                    // Khi đang ở menu chính
                    case StateMenuCaro::WAIT:{
                        // 1. Click vào nút "NEW GAME"
                        if (btn_newGame.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            stateMenu = StateMenuCaro::SELECT_MODE;
                            m_board.ResetBoard(); // Bắt đầu ván mới
                            gameOverSoundPlayed = false;
                        }
                        // 2. Click vào nút "LOAD GAME"
                        else if (btn_loadGame.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            ScanSaveFiles();
                            m_loadFileName = ""; // Xóa tên file đang nhập
                            m_loadInputText.setString(""); // Xóa text hiển thị
                            m_loadErrorText.setString(""); // Xóa thông báo lỗi cũ
                            stateMenu = StateMenuCaro::LOAD_INPUT;
                        }
                        // 3. Click vào nút "ABOUT"
                        else if (btn_about.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            stateMenu = StateMenuCaro::ABOUT_GAME; // Chuyển sang màn hình "About"
                        }
                        // 4. Click vào nút "EXIT"
                        else if (btn_exit.getGlobalBounds().contains(worldPos)){
                            in_game.Stop();
                            mu_game.Stop();
                            so_click.Play();
                            stateMenu = StateMenuCaro::EXIT_GAME;
                            m_window.Destroy();
                        }
                        break;
                    }
                    case StateMenuCaro::SELECT_MODE:{
                        if (btn_pvp.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            modeGame = ModeCaro::P1vsP2;
                            m_board.ResetBoard();
                            m_player1Name = "";
                            m_player2Name = "";
                            m_inputText1.setString("");
                            m_inputText2.setString("");
                            m_currentPlayerInput = 1; // Bắt đầu nhập cho P1
                            stateMenu = StateMenuCaro::INPUT_NAMES;
                        }
                        else if (btn_pvc.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            modeGame = ModeCaro::PvsCPU;
                            m_board.ResetBoard();
                            m_player1Name = "";
                            m_player2Name = "AI";
                            m_inputText1.setString("");
                            m_inputText2.setString("AI");
                            m_currentPlayerInput = 1; // Chỉ cho phép nhập P1
                            stateMenu = StateMenuCaro::INPUT_NAMES;
                        }
                        break;
                    }
                    case StateMenuCaro::SELECT_BOARD_SIZE:{
                        mu_game.Stop();
                        // Click nút 3x3
                        if (btn_3x3.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            m_board.SetModeBoard(BoardMode::MODE_3x3);
                            selectedBoardMode = BoardMode::MODE_3x3;
                            stateMenu = StateMenuCaro::NEW_GAME;
                            m_board.ResetBoard();
                            m_board.ScaleToWindow(m_window);
                            m_window.Destroy();
                            return;
                        }
                        // Click nút 5x5
                        else if (btn_5x5.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            m_board.SetModeBoard(BoardMode::MODE_5x5);
                            selectedBoardMode = BoardMode::MODE_5x5;
                            stateMenu = StateMenuCaro::NEW_GAME;
                            m_board.ResetBoard();
                            m_board.ScaleToWindow(m_window);
                            m_window.Destroy();
                            return;
                        }
                        // Click nút 10x10
                        else if (btn_10x10.getGlobalBounds().contains(worldPos)){
                            so_click.Play();
                            m_board.SetModeBoard(BoardMode::MODE_10x10);
                            selectedBoardMode = BoardMode::MODE_10x10;
                            stateMenu = StateMenuCaro::NEW_GAME;
                            m_board.ResetBoard();
                            m_board.ScaleToWindow(m_window);
                            m_window.Destroy();
                            return;
                        }
                        break;
                    }
                    // case StateMenuCaro::LOAD_INPUT:{
                    //     break;
                    // }
                    // case StateMenuCaro::ABOUT_GAME:
                    //     break;
                    default:
                        break;
                }
            }
        }
    }
}

void MenuCaro::Update(){
    mo_Menu.GetPositionOfMouse(*m_window.GetWindowGame());
    if (stateMenu == StateMenuCaro::EXIT_GAME){
        m_window.Destroy();
    }
}

string MenuCaro::GetPlayer1Name() const{
    // Nếu tên rỗng, trả về tên mặc định
    if (m_player1Name.empty()){
        return "Player 1";
    }
    return m_player1Name;
}

string MenuCaro::GetPlayer2Name() const{
    // Nếu tên rỗng (và không phải AI), trả về tên mặc định
    if (m_player2Name.empty()){
        return "Player 2";
    }
    return m_player2Name;
}

StateMenuCaro MenuCaro::GetStateAfterRun() const{
    return stateMenu;
}

BoardMode MenuCaro::GetBoardMode() const{
    return this->selectedBoardMode;
}

GameStateData MenuCaro::GetLoadedGameData() const{
    return m_loadedGameData;
}

void MenuCaro::ScanSaveFiles(){
    m_saveFiles.clear();
    string savePath = "SaveGame/";
    try{
        if (!filesystem::exists(savePath)){
            filesystem::create_directories(savePath);
        }
        for (const auto& entry : filesystem::directory_iterator(savePath)){
            if (entry.is_regular_file() && entry.path().extension() == ".txt"){
                m_saveFiles.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const filesystem::filesystem_error& e){
        cerr << "Filesystem error scanning saves: " << e.what() << endl;
        m_loadErrorText.setString("Loi: Khong quet duoc thu muc luu!");
    }
    UpdateSaveFileListText();
}

void MenuCaro::UpdateSaveFileListText(){
    string listStr = "Danh sach file da luu (trong folder SaveGame):\n";
    if (m_saveFiles.empty()){
        listStr += "- Khong co file nao.\n";
    }
    else{
        for (const string& filename : m_saveFiles){
            listStr += "- " + filename + "\n";
        }
    }
    m_saveFileListText.setString(listStr);
}

void MenuCaro::DrawMenu(){
    RenderWindow* window = m_window.GetWindowGame();
    window->setView(window->getDefaultView());
    switch (stateMenu){
        case StateMenuCaro::WAIT:
            // Vẽ giao diện menu chính
            m_window.Draw(spr_background);
            m_window.Draw(btn_newGame);
            m_window.Draw(btn_loadGame);
            m_window.Draw(btn_about);
            m_window.Draw(btn_exit);
            // Tô màu
            // btn_color(btn_newGame, m_window.GetWindowGame(), Color(0, 255, 0, 100));
            // btn_color(btn_loadGame, m_window.GetWindowGame(), Color(0, 0, 255, 100));
            // btn_color(btn_about, m_window.GetWindowGame(), Color(255, 255, 0, 100));
            // btn_color(btn_exit, m_window.GetWindowGame(), Color(255, 0, 0, 100));
            break;
        case StateMenuCaro::SELECT_MODE:
            // Vẽ giao diện
            m_window.Draw(spr_background1);
            // Vẽ hai nút chọn chế độ
            m_window.Draw(btn_pvp);
            m_window.Draw(btn_pvc);
            // Tô màu
            // btn_color(btn_pvp, m_window.GetWindowGame(), Color(0, 255, 0, 100));
            // btn_color(btn_pvc, m_window.GetWindowGame(), Color(0, 0, 255, 100));
            // TODO: Nên vẽ thêm một nút "Back" ở đây
            break;
        case StateMenuCaro::SELECT_BOARD_SIZE:
            m_window.Draw(spr_background1);
            // Vẽ ba nút chọn kích thước bàn cờ
            m_window.Draw(btn_3x3);
            m_window.Draw(btn_5x5);
            m_window.Draw(btn_10x10);
            // Tô màu
            // btn_color(btn_3x3, m_window.GetWindowGame(), Color(0, 255, 0, 100));
            // btn_color(btn_5x5, m_window.GetWindowGame(), Color(0, 0, 255, 100));
            // btn_color(btn_10x10, m_window.GetWindowGame(), Color(255, 255, 0, 100));
            // TODO: Nên vẽ thêm một nút "Back" ở đây để quay lại chọn chế độ
            // m_window.Draw(btn_back);
            break;
        case StateMenuCaro::INPUT_NAMES:
            m_window.Draw(spr_background1);
            m_window.Draw(m_promptText1);
            m_window.Draw(m_inputText1);
            m_window.Draw(m_promptText2);
            m_window.Draw(m_promptText2);
            m_window.Draw(m_inputText2);
            break;
        case StateMenuCaro::LOAD_INPUT:
            m_window.Draw(spr_background2);
            m_window.Draw(m_saveFileListText);
            m_window.Draw(m_loadPromptText);
            m_window.Draw(m_loadInputText);
            m_window.Draw(m_loadErrorText);
            break;
        case StateMenuCaro::ABOUT_GAME:
            m_window.Draw(spr_aboutScreen);
            break;
        default:
            break;
    }
    // Vẽ con trỏ chuột
    mo_Menu.DrawMouse(*m_window.GetWindowGame());
}

void MenuCaro::Run(){
    while (!m_window.IsDone()){
        HandleEvents();
        Update();
        m_window.BeginDraw();
        DrawMenu();
        m_window.EndDraw();
    }
}