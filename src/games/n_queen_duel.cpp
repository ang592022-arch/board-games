#include "board_games/games/n_queen_duel.hpp"

#include <stdexcept>

namespace board_games {

NQueenDuel::NQueenDuel(int board_size)
    : board_(board_size, board_size),
      current_player_(1),
      outcome_(Outcome::InProgress),
      history_()
{
    if (board_size < 4 || board_size > 16 || board_size % 2 != 0) {
        throw std::invalid_argument("N-Queen Duel size must be even and between 4 and 16");
    }
}

MoveStatus NQueenDuel::play(int row, int column)
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
    if (is_queen_threatened(row, column)) {
        outcome_ = current_player_ == 1 ? Outcome::PlayerTwoWon : Outcome::PlayerOneWon;
        return MoveStatus::Threatened;
    }

    const int player = current_player_;
    board_.set(column, row, player == 1 ? Cell::QueenOne : Cell::QueenTwo);
    history_.push_back(Move{player, row, column});

    if (history_.size() == static_cast<std::size_t>(board_.width())) {
        outcome_ = player == 1 ? Outcome::PlayerOneWon : Outcome::PlayerTwoWon;
        return MoveStatus::Accepted;
    }

    current_player_ = current_player_ == 1 ? 2 : 1;
    if (!has_legal_move()) {
        outcome_ = player == 1 ? Outcome::PlayerOneWon : Outcome::PlayerTwoWon;
    }
    return MoveStatus::Accepted;
}

bool NQueenDuel::is_queen_threatened(int row, int col) const
{
    int dir[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    int w = board_.width();
    int h = board_.height();

    for(int d = 0; d < 8; d++)
    {
        int nr = row + dir[d][0];
        int nc = col + dir[d][1];
        while(nr >= 1 && nr <= h && nc >= 1 && nc <= w)
        {
            Cell s = board_.get(nc, nr);
            if(s != Cell::Empty)
                return true;
            nr += dir[d][0];
            nc += dir[d][1];
        }
    }
    return false;
}

bool NQueenDuel::has_legal_move() const
{
    for (int row = 1; row <= board_.height(); ++row) {
        for (int column = 1; column <= board_.width(); ++column) {
            if (board_.is_empty(column, row) && !is_queen_threatened(row, column)) {
                return true;
            }
        }
    }
    return false;
}

void NQueenDuel::reset()
{
    board_.make_empty();
    current_player_ = 1;
    outcome_ = Outcome::InProgress;
    history_.clear();
}

} // namespace board_games
