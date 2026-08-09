#ifndef BOARD_GAMES_EXERCISES_MENU_HPP
#define BOARD_GAMES_EXERCISES_MENU_HPP

#include <iosfwd>
#include <map>
#include <string>
#include <utility>

namespace board_games {

using MenuItem = std::pair<int, std::string>;

class Menu {
public:
    Menu();
    Menu(const Menu &other);
    Menu(Menu &&other) noexcept;
    Menu &operator=(const Menu &other);
    Menu &operator=(Menu &&other) noexcept;
    ~Menu();

    static MenuItem make_menu_item(int key, const std::string &value);
    int getSize() const;
    std::string &operator[](int key);
    const std::string &operator[](int key) const;
    Menu &operator+=(const MenuItem &item);
    Menu &operator-=(int key);
    int pick_choice();

    friend std::ostream &operator<<(std::ostream &out, const Menu &menu);

private:
    std::map<int, std::string> choices_;
};

} // namespace board_games

#endif
