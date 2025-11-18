// người code: Hoàng Tiến Dũng - B24DCCN152
#include "human_player.h"

// Constructor: Gọi constructor của lớp cha Player và khởi tạo tham chiếu eventInput
HumanPlayer::HumanPlayer(string n, PieceCaro p, EventInputGame& input, WindowGame& window): 
    Player(n, p), eventInput(input), windowGame(window)
{
        
}

// Destructor ảo (có thể để trống)
HumanPlayer::~HumanPlayer(){
    
}

// Override phương thức GetMove
// *** Quan trọng: Phiên bản này KHÔNG chờ input bên trong hàm này ***
// Nó dựa vào vòng lặp game chính (Game::HandleInput) để phát hiện click
// và trả về tọa độ khi click hợp lệ xảy ra trong lượt của người chơi này.
pair <int, int> HumanPlayer::GetMove(BoardCaro& board){
    // 1. Kiểm tra xem có sự kiện nhấn chuột trái vừa xảy ra không
    //    (Sử dụng trạng thái được cập nhật bởi eventInput.CheckEvent() trong Game::HandleInput)
    if (eventInput.typeInputMouse == TypeInPut::LEFT_MOUSE){
        // 2. Lấy vị trí chuột (pixel) từ cửa sổ
        RenderWindow* window = windowGame.GetWindowGame();
        if (!window){
            cerr << "Loi: Khong co RenderWindow trong HumanPlayer::GetMove" << endl;
            return {-1, -1}; // Trả về nước đi không hợp lệ
        }
        Vector2i mousePosPixel = Mouse::getPosition(*window);

        // 3. Chuyển đổi sang tọa độ thế giới (nếu view có thay đổi)
        Vector2f mousePosWorld = window->mapPixelToCoords(mousePosPixel);

        // 4. Lấy tọa độ ô cờ tương ứng từ đối tượng board
        Vector2i boardCoords = board.GetBoardPos(mousePosWorld); // Trả về {hàng, cột} hoặc {-1, -1}

        // 5. Kiểm tra xem click có hợp lệ không (trong bàn cờ và ô trống)
        if (boardCoords.x != -1 && boardCoords.y != -1 && board.GetPiece(boardCoords.y, boardCoords.x) == PieceCaro::EMPTY){
            // Chỉ trả về tọa độ hợp lệ khi người chơi click đúng ô
            // boardCoords.x là hàng, boardCoords.y là cột
            cout << "[HumanPlayer::GetMove] Click hop le tai: row " << boardCoords.x << ", col " << boardCoords.y << endl;
            return {boardCoords.x, boardCoords.y}; // Trả về {hàng, cột}
        }
        else{
            cout << "[HumanPlayer::GetMove] Click KHONG hop le." << endl;
        }
    }

    // 6. Nếu không có click chuột trái hợp lệ trong lần kiểm tra này,
    //    trả về (-1, -1) để báo hiệu chưa có nước đi.
    //    Vòng lặp game chính sẽ tiếp tục gọi GetMove ở các frame sau.
    return {-1, -1};
}