#include "board_games/games/gomoku.hpp"

#include <stdexcept>

namespace board_games {

GomokuGame::GomokuGame(int board_size)
    : board_(board_size, board_size),
      current_player_(1),
      outcome_(Outcome::InProgress),
      history_()
{
    if (board_size < 5 || board_size > 25 || board_size % 2 == 0) {
        throw std::invalid_argument("Gomoku board size must be odd and between 5 and 25");
    }
}

MoveStatus GomokuGame::play(int row, int column)
{
    if (outcome_ != Outcome::InProgress) {
        return MoveStatus::GameFinished;
    }
    if (!board_.in_bounds(column, row)) {
        return MoveStatus::OutOfBounds;
    }
    if (!board_.is_empty(column, row)) {
        return MoveStatus::Occupied;
    }

    const int player = current_player_;
    const Cell stone = player == 1 ? Cell::Black : Cell::White;
    board_.set(column, row, stone);
    history_.push_back(Move{player, row, column});

    if (check_win_condition(column, row, stone)) {
        outcome_ = player == 1 ? Outcome::PlayerOneWon : Outcome::PlayerTwoWon;
    } else if (board_.full()) {
        outcome_ = Outcome::Draw;
    } else {
        current_player_ = current_player_ == 1 ? 2 : 1;
    }
    return MoveStatus::Accepted;
}

bool GomokuGame::check_win_condition(int x, int y, Cell stone) const
{
    const int dir[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int boardW = board_.width();
    int boardH = board_.height();

    for(int d = 0; d < 4; ++d)
    {
        int count = 1;
        int nx = x + dir[d][0];
        int ny = y + dir[d][1];
        while(nx >= 1 && nx <= boardW && ny >= 1 && ny <= boardH)
        {
            Cell curSt = board_.get(nx, ny);
            if(curSt == stone)
                count++;
            else
                break;
            nx += dir[d][0];
            ny += dir[d][1];
        }

        int rx = x - dir[d][0];
        int ry = y - dir[d][1];
        while(rx >= 1 && rx <= boardW && ry >= 1 && ry <= boardH)
        {
            Cell curSt = board_.get(rx, ry);
            if(curSt == stone)
                count++;
            else
                break;
            rx -= dir[d][0];
            ry -= dir[d][1];
        }

        if(count >= 5)
            return true;
    }
    return false;
}

void GomokuGame::reset()
{
    board_.make_empty();
    current_player_ = 1;
    outcome_ = Outcome::InProgress;
    history_.clear();
}

void GomokuGame::replay(const std::function<void(const Board &, const Move &)> &observer)
{
    const std::vector<Move> history = history_;
    reset();
    for (std::vector<Move>::const_iterator it = history.begin(); it != history.end(); ++it) {
        if (it->player != current_player_ || play(it->row, it->column) != MoveStatus::Accepted) {
            throw std::logic_error("Gomoku history contains an invalid move");
        }
        observer(board_, *it);
    }
}

} // namespace board_games
