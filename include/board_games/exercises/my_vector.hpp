#ifndef BOARD_GAMES_EXERCISES_MY_VECTOR_HPP
#define BOARD_GAMES_EXERCISES_MY_VECTOR_HPP

#include <initializer_list>
#include <stdexcept>

namespace board_games {

enum { DEFAULT_CAPACITY = 10 };

template <typename T>
class MyVector {
public:
    MyVector();
    MyVector(std::initializer_list<T> values);
    MyVector(const MyVector &other);
    MyVector(MyVector &&other) noexcept;
    MyVector &operator=(const MyVector &other);
    MyVector &operator=(MyVector &&other) noexcept;
    ~MyVector();

    void push_back(const T &value);
    void pop_back();
    int size_value() const noexcept { return size; }
    int capacity_value() const noexcept { return capacity; }
    bool empty() const noexcept { return size == 0; }
    T &operator[](int index);
    const T &operator[](int index) const;

private:
    bool debugMode;
    int size;
    int capacity;
    T *memory;

    void resize();
};

} // namespace board_games

#include "board_games/exercises/my_vector.tpp"

#endif
