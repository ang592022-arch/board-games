#include "board_games/core/board.hpp"
#include "board_games/algorithms/n_queen_alg.hpp"
#include "board_games/cli/cli.hpp"
#include "board_games/core/input.hpp"
#include "board_games/core/text.hpp"
#include "board_games/exercises/menu.hpp"
#include "board_games/exercises/my_vector.hpp"
#include "board_games/exercises/stones.hpp"
#include "board_games/games/gomoku.hpp"
#include "board_games/games/n_queen_duel.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {

class Checks {
public:
    void expect(bool condition, const std::string &description)
    {
        if (condition) {
            ++passed_;
            std::cout << "[PASS] " << description << '\n';
        } else {
            ++failed_;
            std::cerr << "[FAIL] " << description << '\n';
        }
    }

    template <typename Actual, typename Expected>
    void equal(const Actual &actual, const Expected &expected, const std::string &description)
    {
        expect(actual == expected, description);
    }

    int finish() const
    {
        std::cout << "\n" << passed_ << " checks passed; " << failed_ << " failed.\n";
        return failed_ == 0 ? 0 : 1;
    }

private:
    int passed_ = 0;
    int failed_ = 0;
};

void check_board(Checks &checks)
{
    using board_games::Board;
    using board_games::Cell;

    Board board(5, 3);
    checks.equal(board.width(), 5, "Board reports its width");
    checks.equal(board.height(), 3, "Board reports its height");
    checks.expect(board.is_empty(5, 3), "A new board is empty");

    board.set(5, 3, Cell::Black);
    checks.expect(board.get(5, 3) == Cell::Black,
                  "A non-square board stores the bottom-right cell safely");
    board.remove_stone(5, 3);
    checks.expect(board.is_empty(5, 3), "remove_stone clears one cell");

    board.set(2, 'b', Cell::White);
    checks.expect(board.get(2, 'B') == Cell::White,
                  "Letter coordinates are case-insensitive");

    bool rejected_i = false;
    try {
        (void)board.get(2, 'I');
    } catch (const std::out_of_range &) {
        rejected_i = true;
    }
    checks.expect(rejected_i, "Letter coordinates skip column I");

    bool rejected_bounds = false;
    try {
        board.set(1, 4, Cell::Black);
    } catch (const std::out_of_range &) {
        rejected_bounds = true;
    }
    checks.expect(rejected_bounds, "Board rejects out-of-range coordinates");

    board.make_empty();
    checks.expect(board.is_empty(2, 2), "make_empty clears the whole board");

    std::ostringstream compact;
    board.render(compact, false);
    checks.expect(compact.str().find("A") != std::string::npos &&
                      compact.str().find("3") != std::string::npos,
                  "Compact rendering includes column and row labels");

    std::ostringstream grid;
    board.render(grid, true);
    checks.expect(grid.str().find('+') != std::string::npos,
                  "Grid rendering includes cell borders");
}

bool play_gomoku_line(board_games::GomokuGame &game,
                      const std::vector<std::pair<int, int> > &line,
                      const std::vector<std::pair<int, int> > &fillers)
{
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (game.play(line[i].first, line[i].second) !=
            board_games::MoveStatus::Accepted) {
            return false;
        }
        if (i < fillers.size()) {
            if (game.play(fillers[i].first, fillers[i].second) !=
                board_games::MoveStatus::Accepted) {
                return false;
            }
        }
    }
    return true;
}

std::string board_signature(const board_games::Board &board)
{
    std::string result;
    for (int row = 1; row <= board.height(); ++row) {
        for (int column = 1; column <= board.width(); ++column) {
            result += board_games::cell_symbol(board.get(column, row));
        }
    }
    return result;
}

bool same_moves(const std::vector<board_games::Move> &left,
                const std::vector<board_games::Move> &right)
{
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i].player != right[i].player ||
            left[i].row != right[i].row ||
            left[i].column != right[i].column) {
            return false;
        }
    }
    return true;
}

void check_gomoku(Checks &checks)
{
    using board_games::Cell;
    using board_games::GomokuGame;
    using board_games::MoveStatus;
    using board_games::Outcome;

    const std::vector<std::pair<int, int> > horizontal = {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}};
    const std::vector<std::pair<int, int> > horizontal_fillers = {
        {5, 1}, {5, 2}, {5, 3}, {5, 4}};
    GomokuGame horizontal_game(5);
    checks.expect(play_gomoku_line(horizontal_game, horizontal, horizontal_fillers),
                  "Horizontal Gomoku setup accepts every move");
    checks.expect(horizontal_game.outcome() == Outcome::PlayerOneWon,
                  "Gomoku detects a horizontal line");

    const std::vector<std::pair<int, int> > vertical = {
        {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}};
    const std::vector<std::pair<int, int> > vertical_fillers = {
        {1, 5}, {2, 5}, {3, 5}, {4, 5}};
    GomokuGame vertical_game(5);
    checks.expect(play_gomoku_line(vertical_game, vertical, vertical_fillers),
                  "Vertical Gomoku setup accepts every move");
    checks.expect(vertical_game.outcome() == Outcome::PlayerOneWon,
                  "Gomoku detects a vertical line");

    const std::vector<std::pair<int, int> > diagonal_down = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    const std::vector<std::pair<int, int> > diagonal_down_fillers = {
        {5, 1}, {5, 2}, {5, 3}, {5, 4}};
    GomokuGame diagonal_down_game(5);
    checks.expect(play_gomoku_line(diagonal_down_game, diagonal_down,
                                   diagonal_down_fillers),
                  "Descending-diagonal Gomoku setup accepts every move");
    checks.expect(diagonal_down_game.outcome() == Outcome::PlayerOneWon,
                  "Gomoku detects a descending diagonal");

    const std::vector<std::pair<int, int> > diagonal_up = {
        {5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5}};
    const std::vector<std::pair<int, int> > diagonal_up_fillers = {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}};
    GomokuGame diagonal_up_game(5);
    checks.expect(play_gomoku_line(diagonal_up_game, diagonal_up,
                                   diagonal_up_fillers),
                  "Ascending-diagonal Gomoku setup accepts every move");
    checks.expect(diagonal_up_game.outcome() == Outcome::PlayerOneWon,
                  "Gomoku detects an ascending diagonal");

    const std::vector<std::pair<int, int> > center_fillers = {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}};
    const std::vector<std::vector<std::pair<int, int> > > center_last_lines = {
        {{5, 3}, {5, 4}, {5, 6}, {5, 7}, {5, 5}},
        {{3, 5}, {4, 5}, {6, 5}, {7, 5}, {5, 5}},
        {{3, 3}, {4, 4}, {6, 6}, {7, 7}, {5, 5}},
        {{7, 3}, {6, 4}, {4, 6}, {3, 7}, {5, 5}}};
    for (std::size_t direction = 0; direction < center_last_lines.size(); ++direction) {
        GomokuGame center_last(9);
        checks.expect(play_gomoku_line(center_last, center_last_lines[direction],
                                       center_fillers),
                      "Center-last Gomoku setup accepts every move");
        checks.expect(center_last.outcome() == Outcome::PlayerOneWon,
                      "Gomoku counts matching stones on both sides of the last move");
    }

    GomokuGame validation_game(5);
    const std::string empty_signature = board_signature(validation_game.board());
    checks.expect(validation_game.play(0, 1) == MoveStatus::OutOfBounds,
                  "Gomoku rejects an out-of-range move");
    checks.equal(validation_game.current_player(), 1,
                 "An out-of-range move does not switch players");
    checks.equal(validation_game.history().size(), static_cast<std::size_t>(0),
                 "An out-of-range move is not recorded");
    checks.equal(board_signature(validation_game.board()), empty_signature,
                 "An out-of-range move does not alter the board");
    checks.expect(validation_game.outcome() == Outcome::InProgress,
                  "An out-of-range move does not alter the outcome");
    checks.expect(validation_game.play(3, 3) == MoveStatus::Accepted,
                  "Gomoku accepts an empty in-range cell");
    checks.equal(validation_game.current_player(), 2,
                 "A valid Gomoku move switches players");
    const std::string one_move_signature = board_signature(validation_game.board());
    const std::size_t one_move_history = validation_game.history().size();
    checks.expect(validation_game.play(3, 3) == MoveStatus::Occupied,
                  "Gomoku rejects an occupied cell");
    checks.equal(validation_game.current_player(), 2,
                 "An occupied move does not switch players");
    checks.equal(validation_game.history().size(), one_move_history,
                 "An occupied move is not recorded");
    checks.equal(board_signature(validation_game.board()), one_move_signature,
                 "An occupied move does not alter the board");

    GomokuGame four_game(5);
    checks.expect(
        play_gomoku_line(
            four_game,
            std::vector<std::pair<int, int> >{{1, 1}, {1, 2}, {1, 3}, {1, 4}},
            std::vector<std::pair<int, int> >{{5, 1}, {5, 2}, {5, 3}}),
        "Four-stone Gomoku setup accepts every move");
    checks.expect(four_game.outcome() == Outcome::InProgress,
                  "Four stones are not a Gomoku win");

    checks.expect(horizontal_game.play(2, 2) == MoveStatus::GameFinished,
                  "Gomoku rejects moves after game over");

    GomokuGame draw_game(5);
    const std::vector<std::pair<int, int> > black_cells = {
        {1,1},{1,2},{1,5},{2,3},{2,4},{3,1},{3,2},
        {3,5},{4,3},{4,4},{5,1},{5,4},{5,5}};
    const std::vector<std::pair<int, int> > white_cells = {
        {1,3},{1,4},{2,1},{2,2},{2,5},{3,3},
        {3,4},{4,1},{4,2},{4,5},{5,2},{5,3}};
    for (std::size_t i = 0; i < black_cells.size(); ++i) {
        draw_game.play(black_cells[i].first, black_cells[i].second);
        if (i < white_cells.size()) {
            draw_game.play(white_cells[i].first, white_cells[i].second);
        }
    }
    checks.expect(draw_game.outcome() == Outcome::Draw,
                  "A full board without five stones is a draw");

    GomokuGame replay_game(5);
    checks.expect(play_gomoku_line(replay_game, horizontal, horizontal_fillers),
                  "Replay setup accepts every Gomoku move");
    const std::string expected_board = board_signature(replay_game.board());
    const std::vector<board_games::Move> expected_history = replay_game.history();
    const int expected_player = replay_game.current_player();
    const Outcome expected_outcome = replay_game.outcome();
    int replayed = 0;
    replay_game.replay([&replayed](const board_games::Board &, const board_games::Move &) {
        ++replayed;
    });
    checks.equal(replayed, static_cast<int>(expected_history.size()),
                 "Gomoku replay reapplies every valid move");
    checks.equal(board_signature(replay_game.board()), expected_board,
                 "Gomoku replay rebuilds the entire board state");
    checks.expect(same_moves(replay_game.history(), expected_history),
                  "Gomoku replay preserves move order and players");
    checks.equal(replay_game.current_player(), expected_player,
                 "Gomoku replay preserves the current player");
    checks.expect(replay_game.outcome() == expected_outcome,
                  "Gomoku replay preserves the terminal outcome");

    int replayed_again = 0;
    replay_game.replay(
        [&replayed_again](const board_games::Board &, const board_games::Move &) {
            ++replayed_again;
        });
    checks.equal(replayed_again, static_cast<int>(expected_history.size()),
                 "A second replay does not duplicate history");
    checks.expect(same_moves(replay_game.history(), expected_history),
                  "Repeated replay keeps exactly one copy of each move");

    replay_game.reset();
    checks.equal(replay_game.history().size(), static_cast<std::size_t>(0),
                 "Gomoku reset clears move history");
    checks.equal(replay_game.current_player(), 1, "Gomoku reset restores Player 1");
    checks.expect(replay_game.outcome() == Outcome::InProgress,
                  "Gomoku reset restores the in-progress state");
    checks.expect(board_signature(replay_game.board()) ==
                      std::string(static_cast<std::size_t>(25), '.'),
                  "Gomoku reset clears the board");
}

void check_n_queen_duel(Checks &checks)
{
    using board_games::MoveStatus;
    using board_games::NQueenDuel;
    using board_games::Outcome;

    NQueenDuel threats(8);
    checks.expect(threats.play(4, 4) == MoveStatus::Accepted,
                  "N-Queen Duel accepts a safe first queen");
    checks.expect(threats.is_queen_threatened(4, 1), "N-Queen detects a row threat");
    checks.expect(threats.is_queen_threatened(1, 4), "N-Queen detects a column threat");
    checks.expect(threats.is_queen_threatened(1, 1), "N-Queen detects upper-left diagonal");
    checks.expect(threats.is_queen_threatened(1, 7), "N-Queen detects upper-right diagonal");
    checks.expect(threats.is_queen_threatened(7, 1), "N-Queen detects lower-left diagonal");
    checks.expect(threats.is_queen_threatened(7, 7), "N-Queen detects lower-right diagonal");

    const std::vector<std::pair<int, int> > threatened_rays = {
        {4, 2}, {4, 6}, {2, 4}, {6, 4},
        {2, 2}, {2, 6}, {6, 2}, {6, 6}};
    for (std::size_t ray = 0; ray < threatened_rays.size(); ++ray) {
        NQueenDuel ray_game(8);
        checks.expect(ray_game.play(4, 4) == MoveStatus::Accepted,
                      "N-Queen ray setup accepts the center queen");
        const int row = threatened_rays[ray].first;
        const int column = threatened_rays[ray].second;
        checks.expect(ray_game.play(row, column) == MoveStatus::Threatened,
                      "N-Queen rejects a threatened square on each of eight rays");
        checks.expect(ray_game.outcome() == Outcome::PlayerOneWon,
                      "A threatened attempt awards the duel to the opponent");
        checks.equal(ray_game.successful_placements(), static_cast<std::size_t>(1),
                     "A threatened attempt does not increase the successful count");
        checks.equal(ray_game.current_player(), 2,
                     "The failed player remains identified after a threatened attempt");
        checks.expect(ray_game.board().is_empty(column, row),
                      "A threatened queen is not written to the board");
        checks.expect(ray_game.play(8, 8) == MoveStatus::GameFinished,
                      "N-Queen rejects play after a threatened terminal attempt");
    }

    NQueenDuel validation(4);
    checks.expect(validation.play(0, 1) == MoveStatus::OutOfBounds,
                  "N-Queen Duel rejects out-of-range placement");
    checks.equal(validation.current_player(), 1,
                 "Rejected N-Queen placement does not switch player");
    checks.expect(validation.play(1, 2) == MoveStatus::Accepted,
                  "N-Queen Duel accepts a legal placement");
    checks.equal(validation.current_player(), 2,
                 "Legal N-Queen placement switches player");
    checks.expect(validation.play(1, 2) == MoveStatus::Occupied,
                  "N-Queen Duel rejects an occupied cell");
    checks.equal(validation.successful_placements(), static_cast<std::size_t>(1),
                 "Rejected placements do not increase the successful count");

    NQueenDuel threatened_loss(4);
    threatened_loss.play(1, 1);
    checks.expect(threatened_loss.play(1, 4) == MoveStatus::Threatened,
                  "A threatened N-Queen placement is rejected");
    checks.equal(threatened_loss.successful_placements(), static_cast<std::size_t>(1),
                 "A threatened attempt is not recorded as a successful queen");
    checks.expect(threatened_loss.outcome() == Outcome::PlayerOneWon,
                  "A threatened attempt loses the duel for the current player");

    NQueenDuel completed(4);
    checks.expect(completed.play(1, 2) == MoveStatus::Accepted, "4-Queen solution move 1 is legal");
    checks.expect(completed.play(2, 4) == MoveStatus::Accepted, "4-Queen solution move 2 is legal");
    checks.expect(completed.play(3, 1) == MoveStatus::Accepted, "4-Queen solution move 3 is legal");
    checks.expect(completed.play(4, 3) == MoveStatus::Accepted, "4-Queen solution move 4 is legal");
    checks.equal(completed.successful_placements(), static_cast<std::size_t>(4),
                 "N-Queen Duel ends after four successful placements");
    checks.expect(completed.outcome() == Outcome::PlayerTwoWon,
                  "The player placing the final safe queen wins");
    checks.expect(completed.play(4, 4) == MoveStatus::GameFinished,
                  "N-Queen rejects moves after the required queens are placed");

    NQueenDuel no_legal_move(4);
    checks.expect(no_legal_move.play(1, 1) == MoveStatus::Accepted,
                  "No-legal-move setup accepts move 1");
    checks.expect(no_legal_move.play(2, 3) == MoveStatus::Accepted,
                  "No-legal-move setup accepts move 2");
    checks.expect(no_legal_move.play(4, 2) == MoveStatus::Accepted,
                  "No-legal-move setup accepts move 3");
    checks.equal(no_legal_move.successful_placements(), static_cast<std::size_t>(3),
                 "No-legal-move ending counts only successful queens");
    checks.expect(no_legal_move.outcome() == Outcome::PlayerOneWon,
                  "The last safe player wins when no legal square remains");
}

bool valid_solution(const board_games::Seq &solution)
{
    for (std::size_t row = 0; row < solution.size(); ++row) {
        for (std::size_t other = row + 1U; other < solution.size(); ++other) {
            const int row_distance = static_cast<int>(other - row);
            const int column_distance = solution[other] - solution[row];
            if (solution[row] == solution[other] ||
                row_distance == column_distance ||
                row_distance == -column_distance) {
                return false;
            }
        }
    }
    return true;
}

void check_n_queen_solver(Checks &checks)
{
    using board_games::NQueenSolver;

    NQueenSolver one(1);
    checks.equal(one.one_solution(), board_games::Seq{0},
                 "The 1-Queen solver finds the only solution");
    checks.equal(one.all_solutions().size(), static_cast<std::size_t>(1),
                 "The 1-Queen solver reports one solution");

    NQueenSolver four(4);
    const board_games::Seq one_of_four = four.one_solution();
    checks.equal(one_of_four.size(), static_cast<std::size_t>(4),
                 "The 4-Queen solver returns four placements");
    checks.expect(valid_solution(one_of_four),
                  "The 4-Queen single solution is non-attacking");
    checks.equal(four.all_solutions().size(), static_cast<std::size_t>(2),
                 "The 4-Queen solver reports the known two solutions");

    NQueenSolver two(2);
    checks.expect(two.one_solution().empty(), "The 2-Queen solver reports no solution");

    NQueenSolver three(3);
    checks.expect(three.all_solutions().empty(), "The 3-Queen solver reports no solutions");

    NQueenSolver five(5);
    checks.equal(five.all_solutions().size(), static_cast<std::size_t>(10),
                 "The 5-Queen solver reports the known ten solutions");

    NQueenSolver eight(8);
    checks.equal(eight.all_solutions().size(), static_cast<std::size_t>(92),
                 "The 8-Queen solver reports the known 92 solutions");

    std::ostringstream rendered;
    four.print_one_solution(rendered);
    checks.expect(rendered.str().find('Q') != std::string::npos,
                  "The solver can print a solution");
}

void check_my_vector(Checks &checks)
{
    board_games::MyVector<int> values;
    checks.equal(values.capacity_value(), 10, "MyVector starts with its original capacity");
    for (int value = 0; value < 11; ++value) {
        values.push_back(value * 3);
    }
    checks.equal(values.size_value(), 11, "MyVector grows past its initial capacity");
    checks.equal(values.capacity_value(), 20, "MyVector doubles capacity when full");
    checks.equal(values[10], 30, "MyVector keeps the element that triggers growth");
    for (int value = 11; value < 20; ++value) {
        values.push_back(value * 3);
    }
    checks.equal(values[19], 57, "MyVector preserves elements while growing");

    board_games::MyVector<int> copied;
    copied = values;
    copied[0] = 99;
    checks.equal(values[0], 0, "MyVector copy operator owns independent storage");
    copied = copied;
    checks.equal(copied[0], 99, "MyVector copy operator handles a self copy");

    board_games::MyVector<int> moved;
    moved = std::move(copied);
    checks.equal(moved.size_value(), 20, "MyVector move operator transfers elements");
    checks.equal(copied.size_value(), 0, "MyVector move operator empties the source");
    checks.equal(copied.capacity_value(), 0, "Moved-from MyVector has zero capacity");
    copied.push_back(7);
    checks.equal(copied[0], 7, "A moved-from MyVector can grow and be reused");

    const int moved_size = moved.size_value();
    const int moved_capacity = moved.capacity_value();
    const int moved_last = moved[19];
    moved = std::move(moved);
    checks.equal(moved.size_value(), moved_size,
                 "MyVector move operator handles a self move");
    checks.equal(moved.capacity_value(), moved_capacity,
                 "MyVector self move preserves capacity");
    checks.equal(moved[19], moved_last,
                 "MyVector self move preserves elements");

    board_games::MyVector<int> empty;
    bool pop_rejected = false;
    try {
        empty.pop_back();
    } catch (const std::underflow_error &) {
        pop_rejected = true;
    }
    checks.expect(pop_rejected, "MyVector rejects pop_back on an empty container");
}

void check_menu(Checks &checks)
{
    board_games::Menu menu;
    menu += board_games::Menu::make_menu_item(1, "Play");
    menu += board_games::Menu::make_menu_item(2, "Exit");
    checks.equal(menu.getSize(), 2, "Menu stores keyed choices");

    board_games::Menu copied = menu;
    copied[1] = "Continue";
    checks.equal(menu[1], std::string("Play"), "Menu copy owns independent entries");
    const board_games::Menu &read_only = menu;
    checks.equal(read_only[99], std::string("Nothing"),
                 "Const Menu lookup returns the missing-key sentinel");

    board_games::Menu moved = std::move(copied);
    checks.equal(moved[1], std::string("Continue"), "Menu move preserves entries");
    moved -= 2;
    checks.equal(moved.getSize(), 1, "Menu removes an entry by key");

    std::istringstream input("99\n1\n");
    std::ostringstream output;
    std::streambuf *old_input = std::cin.rdbuf(input.rdbuf());
    std::streambuf *old_output = std::cout.rdbuf(output.rdbuf());
    const int choice = menu.pick_choice();
    std::cin.rdbuf(old_input);
    std::cout.rdbuf(old_output);
    checks.equal(choice, 1, "Menu retries until a listed choice is entered");
}

void check_stones(Checks &checks)
{
    using board_games::Color;
    using board_games::Shape;
    using board_games::Stones;
    using board_games::Symbol;

    Stones stones(2);
    stones.setStone(0, Symbol{'X', "black"}, Color::Black, Color::None, Shape::CIRCLE);
    stones.setStone(1, Symbol{'O', "white"}, Color::White, Color::None, Shape::SQUARE);
    Stones copied(stones);
    stones.setStone(0, Symbol{'A', "changed"}, Color::Red, Color::None, Shape::TRIANGLE);
    checks.equal(copied.at(0).symbol.glyph, 'X',
                 "Stones copy constructor owns independent storage");

    Stones assigned;
    assigned = copied;
    copied.setStone(1, Symbol{'B', "changed"}, Color::Black, Color::None, Shape::CIRCLE);
    checks.equal(assigned.at(1).symbol.glyph, 'O',
                 "Stones copy operator owns independent storage");

    std::ostringstream output;
    std::streambuf *old_output = std::cout.rdbuf(output.rdbuf());
    board_games::showShape(Shape::TRIANGLE);
    std::cout.rdbuf(old_output);
    checks.equal(output.str(), std::string("TRIANGLE"), "showShape prints the selected shape");

    bool rejected = false;
    try {
        stones.setStone(2, Symbol{'!', "bad"}, Color::None, Color::None, Shape::CIRCLE);
    } catch (const std::out_of_range &) {
        rejected = true;
    }
    checks.expect(rejected, "Stones setStone checks its index");
}

void check_student_utilities(Checks &checks)
{
    checks.equal(board_games::trim("  text \t"), std::string("text"),
                 "trim removes leading and trailing whitespace");
    checks.equal(board_games::trim("   "), std::string(),
                 "trim handles an all-whitespace string");

    std::istringstream input("12trailing text\nnext line\n");
    std::ostringstream output;
    std::streambuf *old_input = std::cin.rdbuf(input.rdbuf());
    std::streambuf *old_output = std::cout.rdbuf(output.rdbuf());
    const int value = board_games::input::read_int("");
    std::string next_line;
    std::getline(std::cin, next_line);
    std::cin.rdbuf(old_input);
    std::cout.rdbuf(old_output);
    checks.equal(value, 12, "read_int accepts a valid integer");
    checks.equal(next_line, std::string("next line"),
                 "read_int clears the remainder of the input line");
}

void check_cli(Checks &checks)
{
    {
        std::istringstream input("0\n");
        std::ostringstream output;
        std::ostringstream errors;
        checks.equal(board_games::run_cli(input, output, errors), 0,
                     "CLI exits successfully from the main menu");
        checks.expect(output.str().find("Gomoku") != std::string::npos &&
                          output.str().find("N-Queen Duel") != std::string::npos &&
                          output.str().find("N-Queen Solver") != std::string::npos,
                      "CLI lists the three supported features");
        checks.expect(output.str().find("4.") == std::string::npos,
                      "CLI lists no unsupported fourth mode");
    }

    {
        std::istringstream input("99\nnot-a-number\n0\n");
        std::ostringstream output;
        std::ostringstream errors;
        board_games::run_cli(input, output, errors);
        checks.expect(output.str().find("Unknown menu choice") != std::string::npos &&
                          output.str().find("Please enter one integer") != std::string::npos,
                      "CLI recovers from unknown and non-integer choices");
    }

    {
        std::istringstream input(
            "1\n5\n"
            "1\n1\n5\n1\n"
            "1\n2\n5\n2\n"
            "1\n3\n5\n3\n"
            "1\n4\n5\n4\n"
            "1\n5\n"
            "0\n0\n");
        std::ostringstream output;
        std::ostringstream errors;
        board_games::run_cli(input, output, errors);
        checks.expect(output.str().find("Player 1 wins") != std::string::npos,
                      "CLI can complete a Gomoku game");
    }

    {
        std::istringstream input(
            "2\n4\n"
            "1\n2\n2\n4\n3\n1\n4\n3\n"
            "0\n");
        std::ostringstream output;
        std::ostringstream errors;
        board_games::run_cli(input, output, errors);
        checks.expect(output.str().find("Player 2 wins") != std::string::npos,
                      "CLI can complete a 4-Queen duel");
    }

    {
        std::istringstream input("3\n4\n1\n0\n");
        std::ostringstream output;
        std::ostringstream errors;
        board_games::run_cli(input, output, errors);
        checks.expect(output.str().find('Q') != std::string::npos,
                      "CLI can print an N-Queen solution");
    }

    {
        std::istringstream input;
        std::ostringstream output;
        std::ostringstream errors;
        checks.equal(board_games::run_cli(input, output, errors), 0,
                     "CLI exits cleanly on end-of-input");
    }
}

} // namespace

int main()
{
    Checks checks;
    check_board(checks);
    check_gomoku(checks);
    check_n_queen_duel(checks);
    check_n_queen_solver(checks);
    check_my_vector(checks);
    check_menu(checks);
    check_stones(checks);
    check_student_utilities(checks);
    check_cli(checks);
    return checks.finish();
}
