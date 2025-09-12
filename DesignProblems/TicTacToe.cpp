#include <iostream>
#include <vector>
#include <memory>
#include <exception>
using namespace std;

/*
  Preserves original design:
    - Strategy pattern for player move behavior
    - State pattern for game states
    - Board with move validation and win detection
    - Two players (X, O) and turn management

  Fixes applied:
    - Added forward declarations
    - Replaced problematic unique_ptr moves for objects that should be shared or referenced
    - Corrected input (cin >> row >> col)
    - Properly check dynamic types for game result
    - Fixed win detection logic
*/

//// Forward declarations
class GameContext;
class Player;
class Board;
class PlayerStrategy;
class GameState;

//// Enums and simple types
enum Symbol { X, O, EMPTY };

class Position {
public:
    int row, col;
    Position(int r = 0, int c = 0) : row(r), col(c) {}
};

//// GAME STATE (State Pattern)
class GameState {
public:
    virtual void next(GameContext* context, Player* player, bool hasWon) = 0;
    virtual bool isGameOver() = 0;
    virtual ~GameState() {}
};

class GameContext {
private:
    unique_ptr<GameState> currentState;
public:
    GameContext(unique_ptr<GameState> state) {
        currentState = std::move(state);
    }

    void setState(unique_ptr<GameState> state) {
        currentState = std::move(state);
    }

    void next(Player* player, bool hasWon) {
        if (currentState)
            currentState->next(this, player, hasWon);
    }

    bool isGameOver() {
        return currentState ? currentState->isGameOver() : true;
    }

    GameState* getCurrentState() {
        return currentState.get();
    }
};

class InProgressState : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override {}
    bool isGameOver() override { return false; }
};

class XWonState : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override {}
    bool isGameOver() override { return true; }
};

class OWonState : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override {}
    bool isGameOver() override { return true; }
};

class XTurnState; // forward
class OTurnState; // forward

class XTurnStateImpl : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override;
    bool isGameOver() override { return false; }
};
class OTurnStateImpl : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override;
    bool isGameOver() override { return false; }
};

class DrawState : public GameState {
public:
    void next(GameContext* context, Player* player, bool hasWon) override {}
    bool isGameOver() override { return true; }
};

//// BOARD
class Board {
private:
    int rows, cols;
    vector<vector<Symbol>> grid;
    int movesMade = 0;
public:
    Board(int r = 3, int c = 3) : rows(r), cols(c), grid(r, vector<Symbol>(c, EMPTY)) {}

    bool isValidMove(const Position& pos) {
        if (pos.row < 0 || pos.col < 0 || pos.row >= rows || pos.col >= cols) return false;
        if (grid[pos.row][pos.col] != EMPTY) return false;
        return true;
    }

    void makeMove(const Position& pos, Symbol symbol) {
        if (isValidMove(pos)) {
            grid[pos.row][pos.col] = symbol;
            ++movesMade;
        }
    }

    bool isFull() const {
        return movesMade >= rows * cols;
    }

    // Check row, col, main diag, anti-diag for win for last move
    bool isWinningCell(const Position& pos, Symbol symbol) {
        int r = pos.row, c = pos.col;
        // Row
        bool rowWin = true;
        for (int j = 0; j < cols; ++j) {
            if (grid[r][j] != symbol) { rowWin = false; break; }
        }
        if (rowWin) return true;

        // Column
        bool colWin = true;
        for (int i = 0; i < rows; ++i) {
            if (grid[i][c] != symbol) { colWin = false; break; }
        }
        if (colWin) return true;

        // Main diagonal (if applicable)
        if (r == c) {
            bool diagWin = true;
            for (int i = 0; i < rows; ++i) {
                if (grid[i][i] != symbol) { diagWin = false; break; }
            }
            if (diagWin) return true;
        }

        // Anti-diagonal (if applicable)
        if (r + c == cols - 1) {
            bool antiDiagWin = true;
            for (int i = 0; i < rows; ++i) {
                int j = cols - 1 - i;
                if (grid[i][j] != symbol) { antiDiagWin = false; break; }
            }
            if (antiDiagWin) return true;
        }

        return false;
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                char ch = (grid[i][j] == X ? 'X' : (grid[i][j] == O ? 'O' : '.'));
                cout << ch << ' ';
            }
            cout << '\n';
        }
    }
};

//// PLAYER STRATEGY (Strategy Pattern)
// Note: we pass Board* instead of moving ownership so the board stays valid
class PlayerStrategy {
public:
    virtual Position makeMove(Board* board) = 0;
    virtual ~PlayerStrategy() {}
};

class HumanPlayerStrategy : public PlayerStrategy {
private:
    string playerName;
    Symbol symbol;
public:
    HumanPlayerStrategy(const string& name, Symbol s) : playerName(name), symbol(s) {}
    Position makeMove(Board* board) override {
        int row, col;
        do {
            cout << playerName << " (" << (symbol==X ? 'X' : 'O') << ") - Enter row col: ";
            if (!(cin >> row >> col)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Try again.\n";
                row = col = -1;
                continue;
            }
        } while (!board->isValidMove(Position(row, col)));
        return Position(row, col);
    }
};

class Player {
private:
    Symbol symbol;
    unique_ptr<PlayerStrategy> strategy;
public:
    Player(Symbol s, unique_ptr<PlayerStrategy> strat) : symbol(s), strategy(std::move(strat)) {}
    Position makeMove(Board* board) { return strategy->makeMove(board); }
    Symbol getSymbol() const { return symbol; }
};

//// Implementations for XTurnStateImpl and OTurnStateImpl
void XTurnStateImpl::next(GameContext* context, Player* player, bool hasWon) {
    if (hasWon) {
        if (player->getSymbol() == X) context->setState(make_unique<XWonState>());
        else context->setState(make_unique<OWonState>());
    } else {
        context->setState(make_unique<OTurnStateImpl>());
    }
}

void OTurnStateImpl::next(GameContext* context, Player* player, bool hasWon) {
    if (hasWon) {
        if (player->getSymbol() == O) context->setState(make_unique<OWonState>());
        else context->setState(make_unique<XWonState>());
    } else {
        context->setState(make_unique<XTurnStateImpl>());
    }
}

//// TIC-TAC-TOE GAME
class TicTacToeGame {
private:
    unique_ptr<Board> board;
    shared_ptr<Player> playerX;
    shared_ptr<Player> playerO;
    shared_ptr<Player> currentPlayer;
    unique_ptr<GameContext> gameContext;
public:
    TicTacToeGame(unique_ptr<PlayerStrategy> xStrategy, unique_ptr<PlayerStrategy> oStrategy, int rows = 3, int cols = 3) {
        board = make_unique<Board>(rows, cols);
        playerX = make_shared<Player>(Symbol(X), std::move(xStrategy));
        playerO = make_shared<Player>(Symbol(O), std::move(oStrategy));
        currentPlayer = playerX; // X starts
        gameContext = make_unique<GameContext>(make_unique<XTurnStateImpl>());
    }

    void play() {
        while (!gameContext->isGameOver()) {
            board->print();
            Position pos = currentPlayer->makeMove(board.get());
            Symbol symbol = currentPlayer->getSymbol();
            board->makeMove(pos, symbol);

            bool hasWon = board->isWinningCell(pos, symbol);
            if (hasWon) {
                // update state via context
                gameContext->next(currentPlayer.get(), true);
                break;
            }
            if (board->isFull()) {
                gameContext->setState(make_unique<DrawState>());
                break;
            }
            // no win and not full -> advance turn
            gameContext->next(currentPlayer.get(), false);
            switchPlayer();
        }
        announceResult();
    }

    void switchPlayer() {
        if (currentPlayer->getSymbol() == X) currentPlayer = playerO;
        else currentPlayer = playerX;
    }

    void announceResult() {
        GameState* state = gameContext->getCurrentState();
        if (dynamic_cast<XWonState*>(state)) {
            cout << "Player X Wins\n";
        } else if (dynamic_cast<OWonState*>(state)) {
            cout << "Player O Wins\n";
        } else if (dynamic_cast<DrawState*>(state)) {
            cout << "It's a draw!\n";
        } else {
            cout << "Game ended\n";
        }
    }
};

int main() {
    auto px = make_unique<HumanPlayerStrategy>("Player X", Symbol(X));
    auto po = make_unique<HumanPlayerStrategy>("Player O", Symbol(O));
    TicTacToeGame game(std::move(px), std::move(po), 3, 3);
    game.play();
    return 0;
}
