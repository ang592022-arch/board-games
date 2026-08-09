#include "board_games/core/board.hpp"
#include "board_games/core/input.hpp"

#include <exception>
#include <iostream>
#include <string>

int main()
{
    using board_games::Board;
    using board_games::Cell;

    Board board(9, 9);

    std::cout << "Initial 9x9 board:\n";
    board.render(std::cout);

    struct InputRecord
    {
        int r;
        char c;
    };
    InputRecord records[3];

    for (int i = 0; i < 3; ++i)
    {
        while (true)
        {
            std::cout << "\nEnter stone " << (i + 1) << " (format: Row Col Color, e.g., '5 D black'): ";
            int row;
            char col;
            std::string color_str;

            std::cin >> row >> col >> color_str;
            if (std::cin.fail())
            {
                board_games::input::clear_input_queue();
                std::cout << "Invalid input format. Please try again.\n";
                continue;
            }
            board_games::input::clear_input_queue();

            if (color_str != "black" && color_str != "white")
            {
                std::cout << "Invalid color. Must be 'black' or 'white'. Please try again.\n";
                continue;
            }

            try
            {
                const Cell cell = color_str == "black" ? Cell::Black : Cell::White;
                board.set(row, col, cell);
                records[i] = {row, col};
                break;
            }
            catch (const std::exception &e)
            {
                std::cout << "Error: " << e.what() << ". Please try again.\n";
            }
        }
    }

    std::cout << "\nBoard after 3 stones placed:\n";
    board.render(std::cout);

    std::cout << "\nRemoving the second stone at (" << records[1].r << " " << records[1].c << ")...\n";
    board.remove_stone(records[1].r, records[1].c);

    std::cout << "\nBoard after removal of the second stone:\n";
    board.render(std::cout);
    return 0;
}
