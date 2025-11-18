// người code: Đỗ Lê Trần Đạt - B24DCCN108
#include "AIplayer.h"

// Random generator
random_device rd;
mt19937 gen(rd());

AIplayer::AIplayer(string n, PieceCaro p, int size): Player(n, p), boardSize(size){ // Gọi constructor lớp cha Player(name, piece)
    // humanPlayer được xác định dựa trên quân cờ của AI
    humanPlayer = (p == PieceCaro::X) ? 2 : 1; // Giả sử X là 1, O là 2 trong logic AI
    // aiPlayer giờ là giá trị của biến 'piece' kế thừa từ lớp cha
}

AIplayer::~AIplayer(){

}

pair<int, int> AIplayer::GetMove(BoardCaro& board) {
    // 1. Lấy trạng thái bàn cờ hiện tại từ đối tượng BoardCaro
    vector<vector<int>> currentBoardState = board.getBoardState(); // Sử dụng hàm đã có

    // 2. Gọi logic AI đã có để tính nước đi (ví dụ gọi hàm getAIMove cũ hoặc trực tiếp các hàm con)
    //    Cần truyền currentBoardState vào hàm tính toán của AI.
    // --- BẮT ĐẦU logic AI (tương tự hàm getAIMove cũ) ---
    if (isBoardEmpty(currentBoardState)) {
        return getRandomOpeningMove();
    }

    if (boardSize == 3) {
        return getBestMove3x3(currentBoardState);
    } else if (boardSize == 5) {
        return getBestMove5x5(currentBoardState);
    } else {
        return getBestMove10x10(currentBoardState);
    }
    // --- KẾT THÚC logic AI ---
}

pair<int, int> AIplayer::getRandomOpeningMove() {
    int center = boardSize / 2;
    
    if (boardSize == 3) {
        // random cho board 3x3: 10% giữa, 90% mấy ô còn lại idk
        std::uniform_int_distribution<> dis(0, 99);
        int randVal = dis(gen);
        
        if (randVal < 10) { // chia tỉ lệ thành dưới 10% vào giữa
            return {1, 1};
        } else {
            // xử lí các ô khác
            vector<pair<int, int>> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
            vector<pair<int, int>> edges = {{0,1}, {1,0}, {1,2}, {2,1}};
            
            std::uniform_int_distribution<> typeDis(0, 9);
            if (typeDis(gen) < 7) { // 70 góc
                std::uniform_int_distribution<> cornerDis(0, corners.size() - 1);
                return corners[cornerDis(gen)];
            } else { // 30 cạnh
                std::uniform_int_distribution<> edgeDis(0, edges.size() - 1);
                return edges[edgeDis(gen)];
            }
        }
    } else {
        // board to hơn: 40 giữa 60 gần giữa
        std::uniform_int_distribution<> dis(0, 9);
        int randVal = dis(gen);
        
        if (randVal < 4) {
            return {center, center};
        } else {
            vector<pair<int, int>> nearCenter;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int r = center + dr;
                    int c = center + dc;
                    if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
                        nearCenter.push_back({r, c});
                    }
                }
            }
            std::uniform_int_distribution<> cellDis(0, nearCenter.size() - 1);
            return nearCenter[cellDis(gen)];
        }
    }
}

// algo board 3x3
pair<int, int> AIplayer::getBestMove3x3(const vector<vector<int>>& board) {
    // kiểm tra các nước quyết định (thắng/chặn)
    pair<int, int> criticalMove = findCriticalMove(board);
    if (criticalMove.first != -1) {
        return criticalMove;
    }
    
    // tỉ lệ 40 chọn nước tốt
    std::uniform_int_distribution<> dis(0, 9);
    if (dis(gen) < 4) {
        return getRandomGoodMove3x3(board);
    }
    
    // k có nước quyết định, chọn bừa (literally)
    return getBestMoveHeuristic(board);
}

pair<int, int> AIplayer::getRandomGoodMove3x3(const vector<vector<int>>& board) {
    vector<pair<int, int>> goodMoves;
    vector<pair<int, int>> bestMoves;
    
    // Tìm tất cả các ô trống và phân loại
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                int score = evaluatePosition(board, r, c);
                if (score > 5000) {
                    bestMoves.push_back({r, c});
                } else {
                    goodMoves.push_back({r, c});
                }
            }
        }
    }
    
    // Ưu tiên best moves > good moves
    if (!bestMoves.empty()) {
        std::uniform_int_distribution<> bestDis(0, bestMoves.size() - 1);
        return bestMoves[bestDis(gen)];
    } else if (!goodMoves.empty()) {
        std::uniform_int_distribution<> goodDis(0, goodMoves.size() - 1);
        return goodMoves[goodDis(gen)];
    }
    
    return {-1, -1};
}

// algo board 10x10
pair<int, int> AIplayer::getBestMove10x10(const vector<vector<int>>& board) {
    // luôn ktra nước quyết định trước
    pair<int, int> criticalMove = findCriticalMove(board);
    if (criticalMove.first != -1) {
        return criticalMove;
    }
    
    // chọn random 25% trong các nước tốt
    std::uniform_int_distribution<> dis(0, 19);
    if (dis(gen) < 5) {
        return getRandomGoodMove10x10(board);
    }
    
    int bestScore = -1000000;
    pair<int, int> bestMove = {-1, -1};
    vector<pair<int, int>> goodMoves;
    vector<pair<int, int>> veryGoodMoves;
    
    vector<pair<int, int>> activeArea = getActiveArea(board); //chỉ xét các ô trong active area nếu k thì nó đánh lung tung loạn hết cả lên
    
    for (const auto& pos : activeArea) {
        int r = pos.first;
        int c = pos.second;
        
        if (board[r][c] == 0) {
            int score = evaluatePosition10x10(board, r, c);
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = {r, c};
            }
            
            // Phân loại các nước tốt
            if (score > 10000) {
                veryGoodMoves.push_back({r, c}); // very good
            } else if (score > 5000) {
                goodMoves.push_back({r, c}); // good
            }
        }
    }
    
    // Có nhiều very good -> chọn random giữa các nước đó
    if (veryGoodMoves.size() >= 2) {
        std::uniform_int_distribution<> randDis(0, 9);
        if (randDis(gen) < 4) {
            std::uniform_int_distribution<> moveDis(0, veryGoodMoves.size() - 1);
            return veryGoodMoves[moveDis(gen)];
        }
    }
    // Có nhiều good -> chọn random giữa các nước đó
    else if (goodMoves.size() >= 3) {
        std::uniform_int_distribution<> randDis(0, 9);
        if (randDis(gen) < 3) {
            std::uniform_int_distribution<> moveDis(0, goodMoves.size() - 1);
            return goodMoves[moveDis(gen)];
        }
    }
    
    // Ko tìm đc nước tốt -> chọn ngẫu nhiên từ active area
    if (bestMove.first == -1 && !activeArea.empty()) {
        std::uniform_int_distribution<> fallbackDis(0, activeArea.size() - 1);
        return activeArea[fallbackDis(gen)];
    }
    
    return bestMove;
}

pair<int, int> AIplayer::getRandomGoodMove10x10(const vector<vector<int>>& board) {
    vector<pair<int, int>> activeArea = getActiveArea(board);
    vector<pair<int, int>> goodMoves;
    vector<pair<int, int>> veryGoodMoves;
    
    // Tìm các nước có điểm cao
    for (const auto& pos : activeArea) {
        int r = pos.first;
        int c = pos.second;
        
        if (board[r][c] == 0) {
            int score = evaluatePosition10x10(board, r, c);
            if (score > 10000) {
                veryGoodMoves.push_back({r, c});
            } else if (score > 2000) {
                goodMoves.push_back({r, c});
            }
        }
    }
    
    // Ưu tiên very good
    if (!veryGoodMoves.empty()) {
        std::uniform_int_distribution<> dis(0, veryGoodMoves.size() - 1);
        return veryGoodMoves[dis(gen)];
    }
    
    // Good moves
    if (!goodMoves.empty()) {
        std::uniform_int_distribution<> dis(0, goodMoves.size() - 1);
        return goodMoves[dis(gen)];
    }
    
    // k có cả 2 -> chọn random trong active area
    if (!activeArea.empty()) {
        std::uniform_int_distribution<> dis(0, activeArea.size() - 1);
        return activeArea[dis(gen)];
    }
    
    return {-1, -1};
}


// xác định active area cho board 10x10
// giảm độ phức tạp khi tính toán để tính next move nhanh hơn
vector<pair<int, int>> AIplayer::getActiveArea(const vector<vector<int>>& board) {
    vector<pair<int, int>> activeArea;
    int margin = 3; // tím kiếm các quân đã đánh trong bk 3
    
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                for (int dr = -margin; dr <= margin; dr++) {
                    for (int dc = -margin; dc <= margin; dc++) {
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < boardSize && nc >= 0 && nc < boardSize) {
                            if (board[nr][nc] != 0) {
                                activeArea.push_back({r, c});
                                goto next_cell; // nhảy đến ô tiếp
                            }
                        }
                    }
                }
            }
            next_cell:;
        }
    }
    

    // ko có ô nào trong active area -> tìm ở gần giữa + random
    if (activeArea.empty()) {
        int center = boardSize / 2;
        for (int dr = -2; dr <= 2; dr++) {
            for (int dc = -2; dc <= 2; dc++) {
                int r = center + dr;
                int c = center + dc;
                if (r >= 0 && r < boardSize && c >= 0 && c < boardSize && board[r][c] == 0) {
                    activeArea.push_back({r, c});
                }
            }
        }
        
        // thêm tính random để tránh đánh y hệt
        std::uniform_int_distribution<> randDis(0, boardSize - 1);
        for (int i = 0; i < 5; i++) {
            int r = randDis(gen);
            
            int c = randDis(gen);
            if (board[r][c] == 0) {
                activeArea.push_back({r, c});
            }
        }
    }
    return activeArea;
}


int AIplayer::evaluatePosition10x10(const vector<vector<int>>& board, int row, int col) {
    int score = 0;
    
    ScoreResult aiScore = evaluateAllDirections(board, row, col, static_cast<int>(piece));
    ScoreResult humanScore = evaluateAllDirections(board, row, col, humanPlayer);
    
    if (aiScore.fiveInRow > 0) return 1000000;
    if (humanScore.fiveInRow > 0) return 950000;
    if (humanScore.openFour > 0) return 90000;
    if (aiScore.openFour > 0) return 80000;
    if (humanScore.closedFour > 0) return 70000;
    if (aiScore.closedFour > 0) return 60000;
    if (humanScore.openThree > 0) return 40000;
    if (aiScore.openThree > 0) return 30000;
    if (humanScore.openTwo > 0) score += 8000;

    // if (aiScore.fiveInRow > 0) return 1000000;
    //if (humanScore.fiveInRow > 0) return 975000;
    //if (humanScore.openFour > 0) return 950000;
    //if (aiScore.openFour > 0) return 925000;
    //if (humanScore.closedFour > 0) return 90000;
    //if (aiScore.closedFour > 0) return 75000;
    //if (humanScore.openThree > 0) return 60000;
    //if (aiScore.openThree > 0) return 50000;
    //if (humanScore.openTwo > 0) score += 10000;
    
    score += evaluatePositionValue10x10(row, col);
    score += aiScore.potential * 10;
    score += humanScore.potential * 15;
    
    return score;
}

ScoreResult AIplayer::evaluateAllDirections(const vector<vector<int>>& board, int row, int col, int player) {
    ScoreResult result;
    const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
    
    for (int d = 0; d < 4; d++) {
        PatternScore pattern = evaluatePatternInDirection(board, row, col, directions[d][0], directions[d][1], player);
        
        result.fiveInRow += pattern.fiveInRow;
        result.openFour += pattern.openFour;
        result.closedFour += pattern.closedFour;
        result.openThree += pattern.openThree;
        result.closedThree += pattern.closedThree;
        result.openTwo += pattern.openTwo;
        result.potential += pattern.potential;
    }
    
    return result;
}

// đánh giá pattern trong 1 hướng
PatternScore AIplayer::evaluatePatternInDirection(const vector<vector<int>>& board, int row, int col, int dx, int dy, int player) {
    PatternScore score;
    
    for (int start = -5; start <= 0; start++) {
        int consecutive = 0; // số quân liên tiếp
        int emptyBefore = 0; // số ô trống trước chuỗi
        int emptyAfter = 0; // số ô trống sau chuỗi
        bool validPattern = true;
        
        // kiểm tra chuỗi 6 ô
        for (int i = 0; i < 6; i++) {
            int r = row + (start + i) * dx;
            int c = col + (start + i) * dy;
            
            if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
                if (board[r][c] == player || (r == row && c == col)) {
                    consecutive++;
                } else if (board[r][c] == 0) {
                    if (consecutive == 0) emptyBefore++; // đếm ô trống trước chuỗi
                    else emptyAfter++; // đếm ô trống sau chuỗi
                } else {
                    validPattern = false; // đối thủ chặn
                    break;
                }
            } else {
                validPattern = false; // ra ngoài biên -> không hợp lệ
                break;
            }
        }
        
        // nếu hợp lệ và có ít nhất 2 quân, phân loại 
        if (validPattern && consecutive >= 2) {
            if (consecutive >= 5) {
                score.fiveInRow++;
            } else if (consecutive == 4) {
                if (emptyBefore > 0 && emptyAfter > 0) score.openFour++;
                else if (emptyBefore > 0 || emptyAfter > 0) score.closedFour++;
            } else if (consecutive == 3) {
                if (emptyBefore > 0 && emptyAfter > 0) score.openThree++;
                else if (emptyBefore > 0 || emptyAfter > 0) score.closedThree++;
            } else if (consecutive == 2) {
                if (emptyBefore > 0 && emptyAfter > 0) score.openTwo++;
            }
            
            score.potential += consecutive * consecutive;
        }
    }
    
    return score;
}

int AIplayer::evaluatePositionValue10x10(int row, int col) {
    int center = boardSize / 2;
    int dist = abs(row - center) + abs(col - center);
    int maxDist = center * 2;
    int positionScore = (maxDist - dist) * 50;
    return positionScore;
}

// tìm nước đi quyết định: thắng hoặc chặn xong thắng
// ưu tiên: đi 1 nước thắng ngay -> chặn thắng của người chơi -> người chơi có chuỗi 4 thì chặn để def
pair<int, int> AIplayer::findCriticalMove(const vector<vector<int>>& board) {
    // check xem thắng luôn đc k
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                vector<vector<int>> testBoard = board;
                testBoard[r][c] = static_cast<int>(piece);
                if (checkWin(testBoard, static_cast<int>(piece))) {
                    return {r, c};
                }
            }
        }
    }
    
    // check xem người chơi thắng luôn đc k để chặn
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                vector<vector<int>> testBoard = board;
                testBoard[r][c] = humanPlayer;
                if (checkWin(testBoard, humanPlayer)) {
                    return {r, c};
                }
            }
        }
    }
    
    // ktra các chuỗi 4 của người chơi để chặn
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                if (isBlockingFourInRow(board, r, c)) {
                    return {r, c};
                }
            }
        }
    }
    
    return {-1, -1};
}

// Kiểm tra xem nước đi tại x,y có chặn 4 đc ko
bool AIplayer::isBlockingFourInRow(const vector<vector<int>>& board, int row, int col) {
    const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
    
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        
        for (int start = -4; start <= 0; start++) {
            int humanCount = 0;
            int emptyCount = 0;
            int emptyPos = -1;
            bool valid = true;
            
            for (int i = 0; i < 5; i++) {
                int r = row + (start + i) * dx;
                int c = col + (start + i) * dy;
                
                if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
                    if (board[r][c] == humanPlayer) {
                        humanCount++;
                    } else if (board[r][c] == 0) {
                        emptyCount++;
                        if (r == row && c == col) {
                            emptyPos = i;
                        }
                    } else if (board[r][c] == static_cast<int>(piece)) {
                        valid = false;
                        break;
                    }
                } else {
                    valid = false;
                    break;
                }
            }
            
            if (valid && humanCount == 4 && emptyCount == 1 && emptyPos != -1) {
                return true;
            }
        }
    }
    
    return false;
}

// check win tùy theo size board, 3 liên tiếp với 3x3, 5 liên tiếp với 5x5 và 10x10
// check theo 4 hướng
bool AIplayer::checkWin(const vector<vector<int>>& board, int player) {
    const int winLength = (boardSize == 3) ? 3 : 5;
    
    // ngang
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c <= boardSize - winLength; c++) {
            bool win = true;
            for (int i = 0; i < winLength; i++) {
                if (board[r][c + i] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    
    // dọc
    for (int r = 0; r <= boardSize - winLength; r++) {
        for (int c = 0; c < boardSize; c++) {
            bool win = true;
            for (int i = 0; i < winLength; i++) {
                if (board[r + i][c] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    
    // chéo "\"
    for (int r = 0; r <= boardSize - winLength; r++) {
        for (int c = 0; c <= boardSize - winLength; c++) {
            bool win = true;
            for (int i = 0; i < winLength; i++) {
                if (board[r + i][c + i] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    

    // chéo "/"
    for (int r = 0; r <= boardSize - winLength; r++) {
        for (int c = winLength - 1; c < boardSize; c++) {
            bool win = true;
            for (int i = 0; i < winLength; i++) {
                if (board[r + i][c - i] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }
    
    return false;
}

// algo board 5x5
pair<int, int> AIplayer::getBestMove5x5(const vector<vector<int>>& board) {
    pair<int, int> criticalMove = findCriticalMove(board);
    if (criticalMove.first != -1) {
        return criticalMove;
    }
    return getBestMoveHeuristic(board);
}

// algo board general, best move random heuristic
pair<int, int> AIplayer::getBestMoveHeuristic(const vector<vector<int>>& board) {
    int bestScore = -1000000;
    pair<int, int> bestMove = {-1, -1};
    
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] == 0) {
                int score = evaluatePosition(board, r, c);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {r, c};
                }
            }
        }
    }
    
    return bestMove;
}

// đánh giá vị trí tấn công cơ bản
// ờm về cơ bản thì nó đánh giá theo vị trí, atk hay def, điểm def cao hơn atk
int AIplayer::evaluatePosition(const vector<vector<int>>& board, int row, int col) {
    int score = 0;
    
    // đánh giá vị trí
    int center = boardSize / 2;
    int distFromCenter = abs(row - center) + abs(col - center);
    score += (boardSize - distFromCenter) * 10;
    
    const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
    
    int aiMaxThreat = 0; // chuỗi dài nhất ai có thể tạo đc
    int humanMaxThreat = 0; // chuỗi dài nhất người chơi có thể tạo đc
    
    // đánh giá theo 4 hướng
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        
        int aiThreat = evaluateThreatInDirection(board, row, col, dx, dy, static_cast<int>(piece));
        int humanThreat = evaluateThreatInDirection(board, row, col, dx, dy, humanPlayer);
        
        aiMaxThreat = max(aiMaxThreat, aiThreat);
        humanMaxThreat = max(humanMaxThreat, humanThreat);
    }
    
    // tính điểm tấn công của AI
    if (aiMaxThreat >= 5) score += 100000; // win 
    else if (aiMaxThreat == 4) score += 30000;
    else if (aiMaxThreat == 3) score += 5000;
    else if (aiMaxThreat == 2) score += 1000;
    
    // tính điểm phòng thủ (chặn người chơi)
    if (humanMaxThreat >= 5) score += 150000; // chặn thắng luôn
    else if (humanMaxThreat == 4) score += 60000;
    else if (humanMaxThreat == 3) score += 10000;
    else if (humanMaxThreat == 2) score += 2000;
    
    return score;
}

// đánh giá mối đe dọa trong 1 hướng
int AIplayer::evaluateThreatInDirection(const vector<vector<int>>& board, int row, int col, int dx, int dy, int player) {
    int maxConsecutive = 0;
    int windowSize = (boardSize <= 5) ? boardSize : 5;
    
    // duyệt qua các cửa sổ chiều dài windowSize
    for (int start = -(windowSize-1); start <= 0; start++) {
        int consecutive = 0;
        bool validWindow = true;
        
        // kiểm tra cửa sổ
        for (int i = 0; i < windowSize; i++) {
            int r = row + (start + i) * dx;
            int c = col + (start + i) * dy;
            
            if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
                if (board[r][c] == player || (r == row && c == col)) {
                    consecutive++;
                } else if (board[r][c] != 0) {
                    // có quân đối phương trong cửa sổ -> invalid
                    validWindow = false;
                    break;
                }
            } else {
                // ngoài biên -> invalid
                validWindow = false;
                break;
            }
        }
        
        if (validWindow) {
            maxConsecutive = max(maxConsecutive, consecutive);
        }
    }
    
    return maxConsecutive;
}

// check board trống hay k
bool AIplayer::isBoardEmpty(const vector<vector<int>>& board) {
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] != 0) return false;
        }
    }
    return true;
}