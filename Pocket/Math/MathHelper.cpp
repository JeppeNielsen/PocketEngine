#include "MathHelper.hpp"
#include <stdlib.h>
#include <time.h>

using namespace Pocket;

float MathHelper::Random() {
	return (float)rand() / (float)RAND_MAX;
}

float MathHelper::Random(float min, float max) {
	return min + (max - min) * Random();
}

Pocket::Vector2 MathHelper::Random(Vector2 min, Vector2 max) {
	return Vector2(Random(min.x, max.x), Random(min.y, max.y));
}

int MathHelper::Random(int exclusiveMax) {
	return rand() % exclusiveMax;
}

void MathHelper::SeedRandom() {
    SeedRandom((unsigned int)time(NULL));
}

void MathHelper::SeedRandom(unsigned int seed) {
    srand (seed);
}

float MathHelper::PingPong(float t, float min, float max) {    
    float range = max - min;
    float value = fmodf(t + range, range * 2) - range;
    if (value<0)value = -value;
    return min + value;
}

float MathHelper::Bezier(const float& position0, const float& tangent0, const float& position1, const float& tangent1, const float& t) {
    
    float oneMinusT = 1 - t;
    float oneMinusTSquared = oneMinusT * oneMinusT;
    float tSquared = t * t;
    
    return
    position0 * oneMinusTSquared * oneMinusT +
    3 * tangent0 * oneMinusTSquared * t +
    3 * tangent1 * oneMinusT * tSquared +
    position1 * tSquared * t;
    
    //p\x*(1-t)^3+3*p\cx1*(1-t)^2*t+3*p\cx2*(1-t)*t*t+pp\x*t^3
}

float MathHelper::Clamp(float value, float min, float max) {
    if (value>max) return max;
    if (value<min) return min;
    return value;
}
