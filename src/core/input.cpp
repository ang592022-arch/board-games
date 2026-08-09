#include "board_games/core/input.hpp"

#include <cstdio>
#include <iostream>

namespace board_games {
namespace input {

void clear_input_queue()
{
    std::cin.clear();
    int ch;
    while ((ch = std::cin.get()) != '\n' && ch != EOF)
    {
    }
    if (ch == EOF)
    {
        std::cin.clear();
    }
}

int read_int(const std::string &prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            clear_input_queue();
            return value;
        }
        else
        {
            clear_input_queue();
        }
    }
}

} // namespace input
} // namespace board_games
