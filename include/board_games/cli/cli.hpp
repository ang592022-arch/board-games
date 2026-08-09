#ifndef BOARD_GAMES_CLI_CLI_HPP
#define BOARD_GAMES_CLI_CLI_HPP

#include <iosfwd>

namespace board_games {

int run_cli(std::istream &in, std::ostream &out, std::ostream &errors);

} // namespace board_games

#endif
