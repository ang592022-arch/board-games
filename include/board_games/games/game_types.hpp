#ifndef BOARD_GAMES_GAMES_GAME_TYPES_HPP
#define BOARD_GAMES_GAMES_GAME_TYPES_HPP

namespace board_games {

struct Move {
    int player;
    int row;
    int column;
};

enum class MoveStatus {
    Accepted,
    OutOfBounds,
    Occupied,
    Threatened,
    GameFinished
};

enum class Outcome {
    InProgress,
    Draw,
    PlayerOneWon,
    PlayerTwoWon
};

inline int winner(Outcome outcome) noexcept
{
    if (outcome == Outcome::PlayerOneWon) {
        return 1;
    }
    if (outcome == Outcome::PlayerTwoWon) {
        return 2;
    }
    return 0;
}

} // namespace board_games

#endif
