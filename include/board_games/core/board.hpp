#ifndef BOARD_GAMES_CORE_BOARD_HPP
#define BOARD_GAMES_CORE_BOARD_HPP

#include <iosfwd>
#include <utility>
#include <vector>

namespace board_games {

enum class Cell {
    Empty,
    Black,
    White,
    QueenOne,
    QueenTwo
};

char cell_symbol(Cell cell) noexcept;

class Board {
public:
    Board(int width, int height, Cell fill = Cell::Empty);

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    bool in_bounds(int x, int y) const noexcept;

    Cell get(int x, int y) const;
    Cell get(int row, char column) const;
    void set(int x, int y, Cell cell);
    void set(int row, char column, Cell cell);
    void remove_stone(int x, int y);
    void remove_stone(int row, char column);
    void make_empty();
    bool is_empty(int x, int y) const;
    bool full() const noexcept;

    void render(std::ostream &out, bool with_grid = true) const;

private:
    int width_;
    int height_;
    std::vector<Cell> cells_;

    void validate_xy(int x, int y) const;
    std::pair<int, int> to_xy(int row, char column) const;
    std::size_t index_of(int x, int y) const noexcept;
};

} // namespace board_games

#endif
