#include "board_games/core/board.hpp"

#include <algorithm>
#include <cctype>
#include <ostream>
#include <stdexcept>
#include <string>

namespace board_games {
namespace {

std::pair<int, int> center_padding(int field_width, int text_width)
{
    const int padding = std::max(0, field_width - text_width);
    return std::make_pair(padding / 2, padding - padding / 2);
}

std::string column_label(int x)
{
    char label = static_cast<char>('A' + x - 1);
    if (label >= 'I') {
        ++label;
    }
    return std::string(1, label);
}

} // namespace

char cell_symbol(Cell cell) noexcept
{
    switch (cell) {
    case Cell::Black:
        return 'X';
    case Cell::White:
        return 'O';
    case Cell::QueenOne:
        return 'Q';
    case Cell::QueenTwo:
        return 'q';
    case Cell::Empty:
    default:
        return '.';
    }
}

Board::Board(int width, int height, Cell fill)
    : width_(width), height_(height), cells_()
{
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Board dimensions must be positive");
    }
    cells_.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), fill);
}

bool Board::in_bounds(int x, int y) const noexcept
{
    return x >= 1 && x <= width_ && y >= 1 && y <= height_;
}

void Board::validate_xy(int x, int y) const
{
    if (x < 1 || x > width_ || y < 1 || y > height_)
    {
        throw std::out_of_range("Board coordinates out of bounds");
    }
}

std::pair<int, int> Board::to_xy(int row, char col) const
{
    col = static_cast<char>(std::toupper(static_cast<unsigned char>(col)));
    if (col < 'A' || col > 'Z' || col == 'I')
    {
        throw std::out_of_range("Invalid column label");
    }
    int x = col - 'A' + 1;
    if (col > 'I') {
        x -= 1;
    }
    validate_xy(x, row);
    return {x, row};
}

std::size_t Board::index_of(int x, int y) const noexcept
{
    return static_cast<std::size_t>(y - 1) * static_cast<std::size_t>(width_) +
           static_cast<std::size_t>(x - 1);
}

Cell Board::get(int x, int y) const
{
    validate_xy(x, y);
    return cells_[index_of(x, y)];
}

Cell Board::get(int row, char column) const
{
    const std::pair<int, int> xy = to_xy(row, column);
    return get(xy.first, xy.second);
}

void Board::set(int x, int y, Cell cell)
{
    validate_xy(x, y);
    cells_[index_of(x, y)] = cell;
}

void Board::set(int row, char column, Cell cell)
{
    const std::pair<int, int> xy = to_xy(row, column);
    set(xy.first, xy.second, cell);
}

void Board::remove_stone(int x, int y)
{
    validate_xy(x, y);
    cells_[index_of(x, y)] = Cell::Empty;
}

void Board::remove_stone(int row, char column)
{
    const std::pair<int, int> xy = to_xy(row, column);
    remove_stone(xy.first, xy.second);
}

void Board::make_empty()
{
    for (int x = 1; x <= width_; ++x)
    {
        for (int y = 1; y <= height_; ++y)
        {
            cells_[index_of(x, y)] = Cell::Empty;
        }
    }
}

bool Board::is_empty(int x, int y) const
{
    return get(x, y) == Cell::Empty;
}

bool Board::full() const noexcept
{
    return std::find(cells_.begin(), cells_.end(), Cell::Empty) == cells_.end();
}

void Board::render(std::ostream &os, bool with_grid) const
{
    const int field_width = 3;
    const auto print_centered_text = [&](const std::string& text, int field_width)
    {
        auto pad = center_padding(field_width, (int)text.size());
        int left_padding = pad.first;
        int right_padding = pad.second;
        os << std::string(left_padding, ' ') << text << std::string(right_padding, ' ');
    };

    const auto print_cell = [&](int x, int y)
    {
        print_centered_text(std::string(1, cell_symbol(get(x, y))), field_width);
    };

    os << "   ";
    for (int x = 1; x <= width_; ++x) {
        print_centered_text(column_label(x), field_width + (with_grid ? 1 : 0));
    }
    os << '\n';

    if (!with_grid)
    {
        for (int y = 1; y <= height_; ++y)
        {
            os << (y < 10 ? " " : "") << y << ' ';
            for (int x = 1; x <= width_; ++x)
            {
                print_cell(x, y);
            }
            os << '\n';
        }
        return;
    }

    const std::string border = "   +" +
        std::string(static_cast<std::size_t>(width_) * 4U - 1U, '-') + "+\n";
    for (int y = 1; y <= height_; ++y) {
        os << border;
        os << (y < 10 ? " " : "") << y << " |";
        for (int x = 1; x <= width_; ++x) {
            print_cell(x, y);
            os << '|';
        }
        os << '\n';
    }
    os << border;
}

} // namespace board_games
