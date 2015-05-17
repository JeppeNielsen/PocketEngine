#include "Menu.hpp"
#include <algorithm>

using namespace Pocket;

Menu::Menu() : ActiveMenu(this) {}

void Menu::Reset() {
    ActiveMenu = "";
}
