#include "Engine.hpp"
#include "Game.hpp"


int main() {
    Engine e;
    e.Start<Game>(600,650, false);
	return 0;
}