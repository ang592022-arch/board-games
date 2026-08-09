#ifndef BOARD_GAMES_CORE_INPUT_HPP
#define BOARD_GAMES_CORE_INPUT_HPP

#include <string>

namespace board_games {
namespace input {

void clear_input_queue();
int read_int(const std::string &prompt);

} // namespace input
} // namespace board_games

#endif
