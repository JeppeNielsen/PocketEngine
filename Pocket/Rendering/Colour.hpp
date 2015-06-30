#pragma once
#include <string>
#include <sstream>

namespace Pocket {
	class Colour {
	public:
		typedef unsigned char Component;

		Component r;
		Component g;
		Component b;
		Component a;


		Colour();
		~Colour();
        Colour(const Colour& other);

		Colour(Component r, Component g, Component b);
		Colour(Component r, Component g, Component b, Component a);
        Colour(float r, float g, float b);
		Colour(float r, float g, float b, float a);
        Colour(float colorValues, float a);
        Colour(float colorValues);

		bool operator==(const Colour &other) const;
		bool operator!=(const Colour &other) const;

		float R();
		float G();
		float B();
		float A();
        
        void R(float red);
		void G(float green);
		void B(float blue);
		void A(float alpha);

		static const Colour HslToRgb(double Hue, double Saturation, double Lightness, double alpha);
		static Colour Lerp(const Colour& source, const Colour& dest, const float& amount);

		static Colour White();
		static Colour White(float alpha);
		static Colour Black();
		static Colour Black(float alpha);
		static Colour Red();
		static Colour Red(float alpha);
		static Colour Green();
		static Colour Green(float alpha);
		static Colour Blue();
		static Colour Blue(float alpha);
		static Colour Yellow();
		static Colour Yellow(float alpha);
        
        Colour static Deserialize(const std::string& data);
        Colour operator *(const Colour &other) const;
	};
}

std::ostream& operator<<(std::ostream& stream, const Pocket::Colour& v);

