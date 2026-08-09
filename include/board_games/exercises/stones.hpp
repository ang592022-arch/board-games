#ifndef BOARD_GAMES_EXERCISES_STONES_HPP
#define BOARD_GAMES_EXERCISES_STONES_HPP

#include <string>

namespace board_games {

enum class Color { None, Black, White, Red };
enum class Shape { CIRCLE, SQUARE, TRIANGLE };

struct Symbol {
    char glyph;
    std::string name;
};

struct Stone {
    Symbol symbol;
    Color symbolColor;
    Color backgroundColor;
    Shape shape;
};

void showShape(Shape shape);

class Stones {
public:
    explicit Stones(int count = 0);
    Stones(const Stones &other);
    Stones &operator=(const Stones &other);
    ~Stones();

    int size() const noexcept { return stoneNum; }
    Stone &at(int index);
    const Stone &at(int index) const;
    void setStone(int index, const Symbol &newSymbol, Color symColor,
                  Color bgColor, Shape shp);

private:
    Stone *stoneArr;
    int stoneNum;
};

} // namespace board_games

#endif
