#ifndef BOARD_GAMES_GAMES_N_QUEEN_DUEL_HPP
#define BOARD_GAMES_GAMES_N_QUEEN_DUEL_HPP

#include "board_games/core/board.hpp"
#include "board_games/games/game_types.hpp"

#include <cstddef>
#include <vector>

namespace board_games {

class NQueenDuel {
public:
    explicit NQueenDuel(int board_size = 8);

    const Board &board() const noexcept { return board_; }
    int current_player() const noexcept { return current_player_; }
    Outcome outcome() const noexcept { return outcome_; }
    std::size_t successful_placements() const noexcept { return history_.size(); }
    const std::vector<Move> &history() const noexcept { return history_; }

    MoveStatus play(int row, int column);
    bool is_queen_threatened(int row, int col) const;
    void reset();

private:
    Board board_;
    int current_player_;
    Outcome outcome_;
    std::vector<Move> history_;

    bool has_legal_move() const;
};

} // namespace board_games

#endif
