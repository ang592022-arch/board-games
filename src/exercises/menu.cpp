#include "board_games/exercises/menu.hpp"
#include "board_games/core/input.hpp"

#include <iostream>
#include <utility>

namespace board_games {

MenuItem Menu::make_menu_item(int key, const std::string &value)
{
    return std::make_pair(key, value);
}

Menu::Menu() : choices_()
{
}

Menu::Menu(const Menu &other) : choices_(other.choices_)
{
}

Menu::Menu(Menu &&other) noexcept : choices_(std::move(other.choices_))
{
}

Menu &Menu::operator=(const Menu &other)
{
    if (this != &other) {
        choices_ = other.choices_;
    }
    return *this;
}

Menu &Menu::operator=(Menu &&other) noexcept
{
    if (this != &other) {
        choices_ = std::move(other.choices_);
    }
    return *this;
}

Menu::~Menu()
{
}

int Menu::getSize() const
{
    return static_cast<int>(choices_.size());
}

std::string &Menu::operator[](int key)
{
    std::map<int, std::string>::iterator it = choices_.find(key);
    if (it != choices_.end()) {
        return it->second;
    }

    choices_[key] = "Default";
    return choices_[key];
}

const std::string &Menu::operator[](int key) const
{
    std::map<int, std::string>::const_iterator it = choices_.find(key);
    if (it != choices_.end()) {
        return it->second;
    }
    static const std::string nothing = "Nothing";
    return nothing;
}

Menu &Menu::operator+=(const MenuItem &item)
{
    if (choices_.find(item.first) != choices_.end()) {
        std::cout << "Error: Key " << item.first << " already exists in the menu.\n";
    } else {
        choices_.insert(item);
    }
    return *this;
}

Menu &Menu::operator-=(int key)
{
    choices_.erase(key);
    return *this;
}

int Menu::pick_choice()
{
    while (true) {
        int choice = board_games::input::read_int("Enter your choice: ");
        if (choices_.find(choice) != choices_.end()) {
            return choice;
        }
        std::cout << "Invalid choice. Please try again.\n";
    }
}

std::ostream &operator<<(std::ostream &os, const Menu &menu)
{
    for (std::map<int, std::string>::const_iterator it = menu.choices_.begin();
         it != menu.choices_.end(); ++it) {
        os << it->first << ". " << it->second << "\n";
    }
    return os;
}

} // namespace board_games
