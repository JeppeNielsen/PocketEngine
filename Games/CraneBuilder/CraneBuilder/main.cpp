#include "Game.h"
#include "TestQuadTree.h"

int main() {
    TestQuadTree test;
    test.Test();
    Engine e;
    e.Start<Game>();
	return 0;
}