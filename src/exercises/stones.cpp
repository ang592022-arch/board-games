#include "board_games/exercises/stones.hpp"

#include <iostream>
#include <stdexcept>

namespace board_games {

void showShape(Shape shape)
{
    switch (shape)
    {
    case Shape::CIRCLE:   std::cout << "CIRCLE"; break;
    case Shape::SQUARE:   std::cout << "SQUARE"; break;
    case Shape::TRIANGLE: std::cout << "TRIANGLE"; break;
    default:              std::cout << "UNKNOWN_SHAPE"; break;
    }
}

Stones::Stones(int count) : stoneArr(0), stoneNum(count)
{
    if (count < 0) {
        throw std::invalid_argument("Stone count cannot be negative");
    }
    if (stoneNum > 0) {
        stoneArr = new Stone[stoneNum];
    }
}

Stones::Stones(const Stones &other) : stoneNum(other.stoneNum)
{
    stoneArr = new Stone[stoneNum];
    for (int i = 0; i < stoneNum; ++i)
    {
        stoneArr[i] = other.stoneArr[i];
    }
}

Stones &Stones::operator=(const Stones &other)
{
    if (this != &other) {
        Stone *replacement = other.stoneNum == 0 ? static_cast<Stone *>(0)
                                                   : new Stone[other.stoneNum];
        for (int index = 0; index < other.stoneNum; ++index) {
            replacement[index] = other.stoneArr[index];
        }
        delete[] stoneArr;
        stoneArr = replacement;
        stoneNum = other.stoneNum;
    }
    return *this;
}

Stones::~Stones()
{
    delete[] stoneArr;
}

Stone &Stones::at(int index)
{
    if (index < 0 || index >= stoneNum) {
        throw std::out_of_range("Stone index out of range");
    }
    return stoneArr[index];
}

const Stone &Stones::at(int index) const
{
    if (index < 0 || index >= stoneNum) {
        throw std::out_of_range("Stone index out of range");
    }
    return stoneArr[index];
}

void Stones::setStone(int index, const Symbol &newSymbol, Color symColor,
                      Color bgColor, Shape shp)
{
    if (index < 0 || index >= stoneNum)
    {
        throw std::out_of_range("Stone index out of bounds");
    }
    stoneArr[index].symbol = newSymbol;
    stoneArr[index].symbolColor = symColor;
    stoneArr[index].backgroundColor = bgColor;
    stoneArr[index].shape = shp;
}

} // namespace board_games
