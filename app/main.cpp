#include "board_games/cli/cli.hpp"

#include <iostream>

int main()
{
    return board_games::run_cli(std::cin, std::cout, std::cerr);
}
