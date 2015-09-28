#pragma once//;

#include "Vector2.hpp"

namespace Pocket {
	class MathHelper {
	public:

#define ToRadians(degrees) (0.01745329251994329576923690768489 * degrees)
#define ToDegrees(radians) (57.295779513082320876798154814105 * radians)

		static float Random();
		static float Random(float min, float max);
		static Vector2 Random(Vector2 min, Vector2 max);
        static int Random(int exclusiveMax);
        static void SeedRandom();
        static void SeedRandom(unsigned int seed);

        inline static bool FloatEqual(float a, float b, float tolerance) {
            return fabsf(a - b)<=tolerance;
        }
        
        static float PingPong(float t, float min, float max);
        
        static float Bezier(const float& position0, const float& tangent0, const float& position1, const float& tangent1, const float& t);
        
        static float Clamp(float value, float min, float max);
        
        template<class T>
        static T Min(const T& a, const T& b) {
            return a<b ? a : b;
        }
        
        template<class T>
        static T Max(const T& a, const T& b) {
            return a>b ? a : b;
        }
        
#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define DOUBLE_PI 6.28318530718

        static constexpr float Pi = 3.14159265359;
        static constexpr float RadToDeg = 180.0f / Pi;
        static constexpr float DegToRad = Pi / 180.0f;
	};
}

