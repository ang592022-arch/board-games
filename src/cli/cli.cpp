#include "board_games/cli/cli.hpp"

#include "board_games/algorithms/n_queen_alg.hpp"
#include "board_games/core/board.hpp"
#include "board_games/core/text.hpp"
#include "board_games/games/gomoku.hpp"
#include "board_games/games/n_queen_duel.hpp"

#include <sstream>
#include <string>

namespace board_games {
namespace {

bool read_number(std::istream &in, std::ostream &out,
                 const std::string &prompt, int &value)
{
    std::string line;
    while (true) {
        out << prompt;
        if (!std::getline(in, line)) {
            return false;
        }
        std::istringstream parsed(trim(line));
        char trailing = 0;
        if ((parsed >> value) && !(parsed >> trailing)) {
            return true;
        }
        out << "Please enter one integer.\n";
    }
}

bool read_size(std::istream &in, std::ostream &out, const std::string &prompt,
               int minimum, int maximum, bool require_even, int &size)
{
    while (read_number(in, out, prompt, size)) {
        const bool parity_ok = require_even ? size % 2 == 0 : size % 2 != 0;
        if (size >= minimum && size <= maximum && parity_ok) {
            return true;
        }
        out << "Size must be between " << minimum << " and " << maximum
            << (require_even ? " and even.\n" : " and odd.\n");
    }
    return false;
}

const char *player_label(int player)
{
    return player == 1 ? "Player 1" : "Player 2";
}

void print_outcome(std::ostream &out, Outcome outcome)
{
    if (outcome == Outcome::Draw) {
        out << "Draw.\n";
    } else if (winner(outcome) != 0) {
        out << player_label(winner(outcome)) << " wins.\n";
    }
}

bool play_gomoku(std::istream &in, std::ostream &out)
{
    int size = 0;
    if (!read_size(in, out, "Odd board size (5-25): ", 5, 25, false, size)) {
        return false;
    }
    GomokuGame game(size);

    while (game.outcome() == Outcome::InProgress) {
        game.board().render(out);
        out << player_label(game.current_player())
            << (game.current_player() == 1 ? " [X]\n" : " [O]\n");
        int row = 0;
        if (!read_number(in, out, "Row (0 returns to menu): ", row)) {
            return false;
        }
        if (row == 0) {
            return true;
        }
        int column = 0;
        if (!read_number(in, out, "Column: ", column)) {
            return false;
        }

        const MoveStatus status = game.play(row, column);
        if (status == MoveStatus::OutOfBounds) {
            out << "That position is outside the board.\n";
        } else if (status == MoveStatus::Occupied) {
            out << "That position is already occupied.\n";
        }
    }

    game.board().render(out);
    print_outcome(out, game.outcome());

    int replay_choice = 0;
    if (!read_number(in, out, "Replay valid moves? (1=yes, 0=no): ", replay_choice)) {
        return false;
    }
    if (replay_choice == 1) {
        game.replay([&out](const Board &board, const Move &move) {
            out << player_label(move.player) << " -> row " << move.row
                << ", column " << move.column << '\n';
            board.render(out);
        });
    }
    return true;
}

bool play_n_queen(std::istream &in, std::ostream &out)
{
    int size = 0;
    if (!read_size(in, out, "Even board size (4-16): ", 4, 16, true, size)) {
        return false;
    }
    NQueenDuel game(size);

    while (game.outcome() == Outcome::InProgress) {
        game.board().render(out);
        const int player = game.current_player();
        out << player_label(player) << " places a queen.\n";
        int row = 0;
        if (!read_number(in, out, "Row (0 returns to menu): ", row)) {
            return false;
        }
        if (row == 0) {
            return true;
        }
        int column = 0;
        if (!read_number(in, out, "Column: ", column)) {
            return false;
        }

        const MoveStatus status = game.play(row, column);
        if (status == MoveStatus::OutOfBounds) {
            out << "That position is outside the board.\n";
        } else if (status == MoveStatus::Occupied) {
            out << "That position is already occupied.\n";
        } else if (status == MoveStatus::Threatened) {
            out << player_label(player) << " loses: that square is threatened.\n";
        }
    }

    game.board().render(out);
    print_outcome(out, game.outcome());
    return true;
}

bool run_solver(std::istream &in, std::ostream &out)
{
    int size = 0;
    if (!read_number(in, out, "Board size (1-12): ", size)) {
        return false;
    }
    if (size < 1 || size > 12) {
        out << "Size must be between 1 and 12.\n";
        return true;
    }

    int mode = 0;
    if (!read_number(in, out, "1 = one solution, 2 = all solutions: ", mode)) {
        return false;
    }
    NQueenSolver solver(size);
    if (mode == 1) {
        solver.print_one_solution(out);
    } else if (mode == 2) {
        solver.print_all_solutions(out);
    } else {
        out << "Unknown solver mode.\n";
    }
    return true;
}

} // namespace

int run_cli(std::istream &in, std::ostream &out, std::ostream &errors)
{
    (void)errors;
    while (true) {
        out << "\nBoard Games\n"
            << "1. Gomoku\n"
            << "2. N-Queen Duel\n"
            << "3. N-Queen Solver\n"
            << "0. Exit\n";

        int choice = 0;
        if (!read_number(in, out, "Choice: ", choice)) {
            return 0;
        }
        if (choice == 0) {
            return 0;
        }

        bool input_available = true;
        if (choice == 1) {
            input_available = play_gomoku(in, out);
        } else if (choice == 2) {
            input_available = play_n_queen(in, out);
        } else if (choice == 3) {
            input_available = run_solver(in, out);
        } else {
            out << "Unknown menu choice.\n";
        }
        if (!input_available) {
            return 0;
        }
    }
}

} // namespace board_games
