#include "Engine.hpp"
#include "Game.hpp"
//#include "Tester.hpp"

int main() {
    Engine e;
    e.Start<Game>(1024, 768);////800,600);
	return 0;
}