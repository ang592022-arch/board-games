#ifndef BOARD_GAMES_EXERCISES_MY_VECTOR_TPP
#define BOARD_GAMES_EXERCISES_MY_VECTOR_TPP

#include <iostream>
#include <utility>

namespace board_games {

template <typename T>
MyVector<T>::MyVector()
    : debugMode(false), size(0), capacity(DEFAULT_CAPACITY), memory(new T[capacity])
{
}

template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> values)
    : debugMode(false), size(0), capacity(DEFAULT_CAPACITY), memory(new T[capacity])
{
    for (typename std::initializer_list<T>::const_iterator it = values.begin();
         it != values.end(); ++it) {
        push_back(*it);
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector &other)
    : MyVector()
{
    *this = other;
}

template <typename T>
MyVector<T>::MyVector(MyVector &&other) noexcept
    : MyVector()
{
    *this = std::move(other);
}

template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector &other)
{
    if (debugMode)
    {
        std::cout << "Copy operator called" << std::endl;
    }
    if (this != &other)
    {
        T *new_memory = new T[other.capacity];
        for (int i = 0; i < other.size; ++i)
        {
            new_memory[i] = other.memory[i];
        }
        delete[] memory;
        memory = new_memory;
        size = other.size;
        capacity = other.capacity;
        debugMode = other.debugMode;
    }
    return *this;
}

template <typename T>
MyVector<T> &MyVector<T>::operator=(MyVector &&other) noexcept
{
    if (debugMode)
    {
        std::cout << "Move operator called" << std::endl;
    }
    if (this != &other)
    {
        delete[] memory;
        memory = other.memory;
        size = other.size;
        capacity = other.capacity;
        debugMode = other.debugMode;

        other.memory = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

template <typename T>
MyVector<T>::~MyVector()
{
    delete[] memory;
}

template <typename T>
void MyVector<T>::push_back(const T &value)
{
    if (size + 1 > capacity) {
        resize();
    }
    memory[size] = value;
    ++size;
}

template <typename T>
void MyVector<T>::pop_back()
{
    if (size == 0) {
        throw std::underflow_error("Cannot pop from an empty MyVector");
    }
    --size;
}

template <typename T>
T &MyVector<T>::operator[](int index)
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("MyVector index out of range");
    }
    return memory[index];
}

template <typename T>
const T &MyVector<T>::operator[](int index) const
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("MyVector index out of range");
    }
    return memory[index];
}

template <typename T>
void MyVector<T>::resize()
{
   capacity = (capacity == 0) ? DEFAULT_CAPACITY : capacity * 2;
    T *new_memory = new T[capacity];
    for (int i = 0; i < size; ++i)
    {
        new_memory[i] = memory[i];
    }
    delete[] memory;
    memory = new_memory;
}

} // namespace board_games

#endif
