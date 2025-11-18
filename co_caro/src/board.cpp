// Người code: Hoàng Tiến Dũng - B24DCCN152
#include "board.h"

BoardCaro::BoardCaro():
    boardSize(3),
    cellSize(100),
    currentTurn(PieceCaro::X),
    winner(WinnerCaro::NONE),
    mode(BoardMode::MODE_3x3),
    sBoard(tmp),
    sX(tmp),
    sO(tmp),
    moveCount(0)
{
    LoadImages(
        "assets/images/board3.png", "assets/images/board5.png", "assets/images/board10.png",
        "assets/images/x3.png", "assets/images/x5.png", "assets/images/x10.png",
        "assets/images/o3.png", "assets/images/o5.png", "assets/images/o10.png"
    );
    board.assign(boardSize, vector<PieceCaro>(boardSize, PieceCaro::EMPTY));
    posBoard = Vector2i(50, 50);
    lastMove = Vector2i(-1, -1);
}

BoardCaro::~BoardCaro(){

}

void BoardCaro::LoadImages(
    const string& board3, const string& board5, const string& board10,
    const string& x3, const string& x5, const string& x10,
    const string& o3, const string& o5, const string& o10
){
    LoadTexture(tBoard3, board3);
    LoadTexture(tBoard5, board5);
    LoadTexture(tBoard10, board10);

    LoadTexture(tX3, x3);
    LoadTexture(tX5, x5);
    LoadTexture(tX10, x10);

    LoadTexture(tO3, o3);
    LoadTexture(tO5, o5);
    LoadTexture(tO10, o10);

    tBoard3.setSmooth(false);
    tBoard5.setSmooth(false);
    tBoard10.setSmooth(false);

    tX3.setSmooth(false);
    tX5.setSmooth(false);
    tX10.setSmooth(false);

    tO3.setSmooth(false);
    tO5.setSmooth(false);
    tO10.setSmooth(false);
}

void BoardCaro::ScaleToWindow(WindowGame& windowGame){
    // 1. KIỂM TRA AN TOÀN: Đảm bảo sprite đã có texture trước khi xử lý
    const Texture& boardTexture = sBoard.getTexture();
    if (boardTexture.getSize().x == 0 || boardTexture.getSize().y == 0) {
        cerr << "Loi: Ban co chua duoc load anh truoc khi Scale!\n";
        return;
    }
    cout << "Scaling based on texture of size: " << boardTexture.getSize().x << "x" << boardTexture.getSize().y << endl;

    // 2. Lấy kích thước cần thiết
    Vector2u windowSize = windowGame.GetWindowSize();
    Vector2u textureSize = boardTexture.getSize(); // Dùng .getSize() trên tham chiếu

    // 3. Xác định vùng mục tiêu (ví dụ: cửa sổ trừ đi 100px lề)
    float targetWidth = static_cast<float>(windowSize.x) - 100.f;
    float targetHeight = static_cast<float>(windowSize.y) - 100.f;

    // 4.  SỬA LỖI MÉO HÌNH: Tính một tỉ lệ co giãn duy nhất để giữ bàn cờ vuông
    float scaleX = targetWidth / textureSize.x;
    float scaleY = targetHeight / textureSize.y;
    float scale = min(scaleX, scaleY); // Lấy tỉ lệ nhỏ hơn để đảm bảo vừa vặn

    sBoard.setScale({scale, scale}); // Áp dụng cùng một tỉ lệ cho cả X và Y

    // 5. TỰ ĐỘNG CĂN GIỮA: Căn giữa bàn cờ sau khi co giãn
    FloatRect boardBounds = sBoard.getGlobalBounds();
    // float boardPosX = (windowSize.x - boardBounds.size.x) / 2.0f;
    float boardPosX = 50.f;
    float boardPosY = (windowSize.y - boardBounds.size.y) / 2.0f;
    sBoard.setPosition({boardPosX, boardPosY});
    
    // Cập nhật lại biến vị trí logic của lớp để click chuột cho đúng
    this->posBoard = {(int)boardPosX, (int)boardPosY};

    // 6. TÍNH LẠI KÍCH THƯỚC Ô SAU KHI CO GIÃN (chuẩn SFML 3)
    // Dùng .width từ FloatRect
    float cellWidth = sBoard.getGlobalBounds().size.x / boardSize;
    this->cellSize = static_cast<int>(cellWidth); // Cập nhật lại biến cellSize của lớp

    // 7. CO GIÃN CÁC QUÂN CỜ (bao gồm cả highlight)
    // Đặt điểm neo vào giữa quân cờ để dễ dàng định vị sau này
    sX.setOrigin({sX.getLocalBounds().size.x / 2.f, sX.getLocalBounds().size.y / 2.f});
    sO.setOrigin({sO.getLocalBounds().size.x / 2.f, sO.getLocalBounds().size.y / 2.f});

    // Tính tỉ lệ co giãn cho quân cờ (ví dụ: bằng 80% kích thước ô cho đẹp)
    float pieceScale = (cellWidth * 0.8f) / sX.getLocalBounds().size.x;
    sX.setScale({pieceScale, pieceScale});
    sO.setScale({pieceScale, pieceScale});
}

void BoardCaro::SetModeBoard(BoardMode m){
    this->mode = m;
    switch(mode){
        case BoardMode::MODE_3x3:
            boardSize = 3;
            sBoard.setTexture(tBoard3, true);
            sX.setTexture(tX3, true);
            sO.setTexture(tO3, true);
            break;
        case BoardMode::MODE_5x5:
            boardSize = 5;
            sBoard.setTexture(tBoard5, true);
            sX.setTexture(tX5, true);
            sO.setTexture(tO5, true);
            break;
        case BoardMode::MODE_10x10:
            boardSize = 10;
            sBoard.setTexture(tBoard10, true);
            sX.setTexture(tX10, true);
            sO.setTexture(tO10, true);
            break;
    }
    board.assign(boardSize, vector<PieceCaro>(boardSize, PieceCaro::EMPTY));
    winner = WinnerCaro::NONE;
    currentTurn = PieceCaro::X;
    lastMove = Vector2i(-1, -1);
}

void BoardCaro::DrawBoard(WindowGame* w){
    sBoard.setPosition({(float)posBoard.x, (float)posBoard.y});
    w->Draw(sBoard);
}

void BoardCaro::DrawPieces(WindowGame* w){
    for (int r = 0; r < boardSize; r++){
        for (int c = 0; c < boardSize; c++){
            if (board[r][c] == PieceCaro::EMPTY){
                continue;
            }
            float x = posBoard.x + c * cellSize + cellSize / 2.0f;
            float y = posBoard.y + r * cellSize + cellSize / 2.0f;
            if (board[r][c] == PieceCaro::X){
                sX.setPosition({x, y});
                w->Draw(sX);
            }
            else if (board[r][c] == PieceCaro::O){
                sO.setPosition({x, y});
                w->Draw(sO);
            }
        }
    }
}

void BoardCaro::Draw(WindowGame* w){
    // 1. Gọi hàm để vẽ bàn cờ
    DrawBoard(w);
    // 2. Gọi hàm để vẽ các quân cờ lên trên
    DrawPieces(w);
}

bool BoardCaro::IsInsideBoard(int c, int r) const{
    return (c >= 0 && c < boardSize && r >= 0 && r < boardSize);
}

bool BoardCaro::IsBoardFull() const{
    for (int i = 0; i < boardSize; ++i){
        for (int j = 0; j < boardSize; ++j){
            if (board[i][j] == PieceCaro::EMPTY){
                return false; // Vẫn còn ô trống
            }
        }
    }
    return true; // Không còn ô trống nào
}

bool BoardCaro::PlacePiece(int c, int r){
    // Kiểm tra xem nước đi có hợp lệ không
    if (!IsInsideBoard(c, r) || board[r][c] != PieceCaro::EMPTY || winner != WinnerCaro::NONE) {
        return false; // Nước đi không hợp lệ
    }

    PieceCaro pieceToPlace = currentTurn;
    board[r][c] = pieceToPlace;
    
    // 1. Lưu nước đi này vào undoStack
    undoStack.push({r, c, pieceToPlace});

    // 2. Một khi có nước đi mới, lịch sử redo phải bị xóa
    while (!redoStack.empty()) redoStack.pop();

    // Cập nhật các trạng thái khác
    lastMove = Vector2i(c, r); // Vector2i là (x, y)
    moveCount++;
    winner = CheckWin(c, r); // Kiểm tra thắng

    // Nếu game chưa kết thúc, đổi lượt
    if (winner == WinnerCaro::NONE){
        // ... thì mới kiểm tra xem có phải hòa không
        if (moveCount == boardSize * boardSize){
            winner = WinnerCaro::DRAW;
        }
        else{
            SwitchTurn(); // Nếu chưa thắng, chưa hòa thì mới đổi lượt
        }
    }
    
    return true; // Nước đi thành công
}

bool BoardCaro::Undo(){
    // Nếu không có gì để undo, thoát
    if (undoStack.empty()){
        return false;
    }

    // 1. Lấy nước đi cuối cùng ra khỏi undoStack
    Move lastMoveData = undoStack.top();
    undoStack.pop();

    // 2. Thêm nước đi đó vào redoStack
    redoStack.push(lastMoveData);

    // 3. Xóa nước đi đó khỏi bàn cờ
    board[lastMoveData.col][lastMoveData.row] = PieceCaro::EMPTY;

    // 4. Khôi phục trạng thái
    SwitchTurn(); // Đổi lại lượt
    moveCount--;
    winner = WinnerCaro::NONE; // Chắc chắn là game chưa kết thúc

    // 5. Cập nhật highlight về nước đi trước đó nữa
    if (undoStack.empty()){
        lastMove = Vector2i(-1, -1); // Không còn nước nào
    }
    else{
        // Cập nhật lastMove (highlight) về nước đi trên đỉnh stack
        lastMove = Vector2i(undoStack.top().row, undoStack.top().col);
    }
    return true; // Undo thành công
}

bool BoardCaro::Redo(){
    // Nếu không có gì để redo, thoát
    if (redoStack.empty()){
        return false;
    }

    // 1. Lấy nước đi ra khỏi redoStack
    Move moveToRedo = redoStack.top();
    redoStack.pop();

    // 2. Thêm lại vào undoStack
    undoStack.push(moveToRedo);

    // 3. Đặt lại quân cờ lên bàn cờ
    board[moveToRedo.col][moveToRedo.row] = moveToRedo.piece;

    // 4. Cập nhật trạng thái
    lastMove = Vector2i(moveToRedo.row, moveToRedo.col); // Cập nhật highlight
    moveCount++;

    // 5. Kiểm tra lại thắng/hòa và đổi lượt
    if (winner == WinnerCaro::NONE){
        if (IsBoardFull()){
            winner = WinnerCaro::DRAW;
        }
        else{
            SwitchTurn(); // Chỉ đổi lượt nếu game chưa kết thúc
        }
    }
    return true; // Redo thành công
}

PieceCaro BoardCaro::GetPiece(int c, int r) const{
    if(IsInsideBoard(c, r)){
        return board[r][c];
    }
    return PieceCaro::EMPTY;
}

WinnerCaro BoardCaro::CheckWin(int c, int r) const{
    const int winLength = (mode == BoardMode::MODE_3x3) ? 3 : 5;
    PieceCaro current = board[r][c];
    auto countDir = [&](int dx, int dy){
        int cnt = 1;
        int x = c + dx, y = r + dy;
        while(IsInsideBoard(x,y) && board[y][x]==current){
            cnt++;
            x += dx;
            y += dy;
        }
        x = c - dx; y = r - dy;
        while(IsInsideBoard(x,y) && board[y][x]==current){
            cnt++;
            x -= dx;
            y -= dy;
        }
        return cnt;
    };
    // if (countDir(1, 0) >= winLength || countDir(0, 1) >= winLength || countDir(1, 1) >= winLength || countDir(1, -1) >= winLength){
    //     return current == PieceCaro::X ? WinnerCaro::X_WIN : WinnerCaro::O_WIN;
    // }

    // STL: std::array
    const array<Vector2i, 4> directions ={
        Vector2i(1, 0),   // Ngang
        Vector2i(0, 1),   // Dọc
        Vector2i(1, 1),   // Chéo chính
        Vector2i(1, -1)   // Chéo phụ
    };
    // Dùng vòng lặp để duyệt qua các hướng
    for (const auto& dir : directions){
        if (countDir(dir.x, dir.y) >= winLength){
            return current == PieceCaro::X ? WinnerCaro::X_WIN : WinnerCaro::O_WIN;
        }
    }
    return WinnerCaro::NONE;
}

WinnerCaro BoardCaro::GetWinner() const{
    return winner;
}

PieceCaro BoardCaro::GetCurrentTurn() const{
    return currentTurn;
}

void BoardCaro::SwitchTurn(){
    currentTurn = (currentTurn == PieceCaro::X) ? PieceCaro::O : PieceCaro::X;
}

void BoardCaro::ResetBoard(){
    board.assign(boardSize, vector<PieceCaro>(boardSize, PieceCaro::EMPTY));
    winner = WinnerCaro::NONE;
    currentTurn = PieceCaro::X;
    lastMove = Vector2i(-1, -1);
    moveCount = 0;
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}

int BoardCaro::GetBoardSize() const{ 
    return boardSize;
}

Vector2f BoardCaro::getPosition() const{
    // Ép kiểu từ Vector2i (số nguyên) sang Vector2f (số thực)
    return Vector2f(static_cast<float>(posBoard.x), static_cast<float>(posBoard.y));
}

float BoardCaro::getScaledSize() const{
    return static_cast<float>(cellSize * boardSize);
}

const vector <vector <PieceCaro>>& BoardCaro::getGrid() const{
    return this->board;
}

BoardMode BoardCaro::GetModeBoard() const{
    return this->mode;
}

Vector2i BoardCaro::GetBoardPos(const Vector2f& mousePos) const{
    // Lấy bounding box thực tế của bàn cờ sau khi scale
    FloatRect boardBounds = sBoard.getGlobalBounds();

    // Kiểm tra click có nằm trong bàn cờ không
    if (!boardBounds.contains({(float)mousePos.x, (float)mousePos.y})){
        return Vector2i(-1, -1);
    }

    // Tính vị trí tương đối của chuột bên trong bàn cờ
    float relativeX = mousePos.x - boardBounds.position.x;
    float relativeY = mousePos.y - boardBounds.position.y;

    // Mỗi ô có kích thước bao nhiêu
    float cellWidth = boardBounds.size.x / boardSize;
    float cellHeight = boardBounds.size.y / boardSize;

    // Tính hàng và cột
    int col = static_cast<int>(relativeX / cellWidth);
    int row = static_cast<int>(relativeY / cellHeight);

    // Đảm bảo không bị out-of-bound
    if (row >= 0 && row < boardSize && col >= 0 && col < boardSize){
        return Vector2i(row, col);
    }

    return Vector2i(-1, -1);
}

// cập nhật toàn bộ trạng thái của bàn cờ
void BoardCaro::setFromState(const GameStateData& state) {
    // 1. Cập nhật kích thước
    this->boardSize = state.boardSize; // Giả sử bàn cờ vuông

    // 2. Cập nhật lượt chơi (cần ép kiểu từ TurnPlayer sang PieceCaro)
    if (state.nextTurn == TurnPlayer::Player1){
        this->currentTurn = PieceCaro::X;
    }
    else{
        this->currentTurn = PieceCaro::O;
    }

    // 3. Cập nhật trạng thái từng ô cờ
    this->board.assign(boardSize, vector<PieceCaro>(boardSize, PieceCaro::EMPTY));
    for (int i = 0; i < boardSize; ++i){
        for (int j = 0; j < boardSize; ++j){
            // Cần ép kiểu từ int (trong file) sang PieceCaro
            this->board[i][j] = static_cast<PieceCaro>(state.boardState[i][j]);
        }
    }
    
    // 4. Reset các trạng thái khác
    this->winner = WinnerCaro::NONE;
    this->moveCount = 0; // Cần tính lại moveCount nếu muốn chính xác
    for (int i = 0; i < boardSize; ++i){
        for (int j = 0; j < boardSize; ++j){
            if (board[i][j] != PieceCaro::EMPTY){
                this->moveCount++;
            }
        }
    }
}

vector <vector <int>> BoardCaro::getBoardState() const{
    vector<vector<int>> state(boardSize, vector<int>(boardSize, 0));
    for (int r = 0; r < boardSize; r++){
        for (int c = 0; c < boardSize; c++){
            if (board[r][c] == PieceCaro::X){
                state[r][c] = 1;
            }
            else if (board[r][c] == PieceCaro::O){
                state[r][c] = 2;
            }
        }
    }
    return state;
}