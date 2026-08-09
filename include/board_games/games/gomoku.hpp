#ifndef BOARD_GAMES_GAMES_GOMOKU_HPP
#define BOARD_GAMES_GAMES_GOMOKU_HPP

#include "board_games/core/board.hpp"
#include "board_games/games/game_types.hpp"

#include <functional>
#include <vector>

namespace board_games {

class GomokuGame {
public:
    explicit GomokuGame(int board_size = 15);

    const Board &board() const noexcept { return board_; }
    int current_player() const noexcept { return current_player_; }
    Outcome outcome() const noexcept { return outcome_; }
    const std::vector<Move> &history() const noexcept { return history_; }

    MoveStatus play(int row, int column);
    void reset();
    void replay(const std::function<void(const Board &, const Move &)> &observer);

private:
    Board board_;
    int current_player_;
    Outcome outcome_;
    std::vector<Move> history_;

    bool check_win_condition(int x, int y, Cell stone) const;
};

} // namespace board_games

#endif
