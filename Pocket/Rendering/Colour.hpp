#pragma once
#include <string>
#include <sstream>
#include <math.h>

namespace Pocket {
	class Colour {
	public:
		typedef unsigned char Component;

		Component r;
		Component g;
		Component b;
		Component a;

        Colour() {
            r = 255;
            g = 255;
            b = 255;
            a = 255;
        }

        ~Colour() {}

        Colour(Component r, Component g, Component b) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 255;
        }

        Colour(Component r, Component g, Component b, Component a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Colour(const Colour& other) :
        r(other.r),g(other.g),b(other.b),a(other.a) {}

        Colour(float r, float g, float b) {
            this->r = (Component)(r * 255.0f);
            this->g = (Component)(g * 255.0f);
            this->b = (Component)(b * 255.0f);
            this->a = 255;
        }

        Colour(float r, float g, float b, float a) {
            this->r = (Component)(r * 255.0f);
            this->g = (Component)(g * 255.0f);
            this->b = (Component)(b * 255.0f);
            this->a = (Component)(a * 255.0f);
        }

        Colour(float colorValues, float a)
        {
            this->r = (Component)(colorValues * 255.0f);
            this->g = (Component)(colorValues * 255.0f);
            this->b = (Component)(colorValues * 255.0f);
            this->a = (Component)(a * 255.0f);
        }

        Colour(float colorValues)
        {
            this->r = (Component)(colorValues * 255.0f);
            this->g = (Component)(colorValues * 255.0f);
            this->b = (Component)(colorValues * 255.0f);
            this->a = 255;
        }

        bool operator ==(const Colour &other) const {
            return !(r!=other.r || g!=other.g || b!=other.b || a!=other.a);
        }

        bool operator !=(const Colour &other) const {
            return (r!=other.r || g!=other.g || b!=other.b || a!=other.a);
        }

        static const Colour HslToRgb(double Hue, double Saturation, double Lightness, double alpha) {
            double num4 = 0.0;
            double num5 = 0.0;
            double num6 = 0.0;

            while (Hue < 0) {
                Hue += 360;
            }

            double num = fmod(Hue, 360);//fmod( ((double)Hue) % 360.0;
            double num2 = ((double)Saturation);
            double num3 = ((double)Lightness);
            if (num2 == 0.0) {
                num4 = num3;
                num5 = num3;
                num6 = num3;
            }
            else {
                double d = num / 60.0;
                int num11 = (int)floor(d);
                double num10 = d - num11;
                double num7 = num3 * (1.0 - num2);
                double num8 = num3 * (1.0 - (num2 * num10));
                double num9 = num3 * (1.0 - (num2 * (1.0 - num10)));
                switch (num11) {
                case 0:
                    num4 = num3;
                    num5 = num9;
                    num6 = num7;
                    break;
                case 1:
                    num4 = num8;
                    num5 = num3;
                    num6 = num7;
                    break;
                case 2:
                    num4 = num7;
                    num5 = num3;
                    num6 = num9;
                    break;
                case 3:
                    num4 = num7;
                    num5 = num8;
                    num6 = num3;
                    break;
                case 4:
                    num4 = num9;
                    num5 = num7;
                    num6 = num3;
                    break;
                case 5:
                    num4 = num3;
                    num5 = num7;
                    num6 = num8;
                    break;
                }
            }
            return Colour((float)num4, (float)num5, (float)num6, (float)alpha);
        }

        static Colour Lerp(const Colour& source, const Colour& dest, const float amount) {

            float s_r = (((float)source.r) / 255.0f);
            float s_g = (((float)source.g) / 255.0f);
            float s_b = (((float)source.b) / 255.0f);
            float s_a = (((float)source.a) / 255.0f);

            float d_r = (((float)dest.r) / 255.0f);
            float d_g = (((float)dest.g) / 255.0f);
            float d_b = (((float)dest.b) / 255.0f);
            float d_a = (((float)dest.a) / 255.0f);

            return Colour(
                s_r + (d_r - s_r) * amount,
                s_g + (d_g - s_g) * amount,
                s_b + (d_b - s_b) * amount,
                s_a + (d_a - s_a) * amount);
        }

        float R() const { return (float)r / 255.0f; }
        float G() const { return (float)g / 255.0f; }
        float B() const { return (float)b / 255.0f; }
        float A() const { return (float)a / 255.0f; }

        void R(float red) { r = (Component)(red * 255.0f); }
        void G(float green) { g = (Component)(green * 255.0f); }
        void B(float blue) { b = (Component)(blue * 255.0f);}
        void A(float alpha) { a = (Component)(alpha * 255.0f); }

        static Colour White() { return White(1.0f); }
        static Colour White( float alpha ) { return Colour(1.0f,1.0f,1.0f,alpha); }

        static Colour Black() { return Black(1.0f); }
        static Colour Black( float alpha ) { return Colour(0.0f,0.0f,0.0f,alpha); }

        static Colour Red() { return Red(1.0f); }
        static Colour Red( float alpha ) { return Colour(1.0f,0.0f,0.0f,alpha); }

        static Colour Green() { return Green(1.0f); }
        static Colour Green( float alpha ) { return Colour(0.0f,1.0f,0.0f,alpha); }

        static Colour Blue() { return Blue(1.0f); }
        static Colour Blue( float alpha ) { return Colour(0.0f,0.0f,1.0f,alpha); }

        static Colour Yellow() { return Yellow(1.0f); }
        static Colour Yellow( float alpha ) { return Colour(1.0f,1.0f,0.0f,alpha); }


        Colour operator *(const Colour &other) const {
            float s_r = (((float)r) / 255.0f);
            float s_g = (((float)g) / 255.0f);
            float s_b = (((float)b) / 255.0f);
            float s_a = (((float)a) / 255.0f);
            
            float d_r = (((float)other.r) / 255.0f);
            float d_g = (((float)other.g) / 255.0f);
            float d_b = (((float)other.b) / 255.0f);
            float d_a = (((float)other.a) / 255.0f);
            return Colour(s_r*d_r, s_g*d_g, s_b*d_b, s_a*d_a);
        }
		
        Colour static Deserialize(const std::string& data);
        friend std::ostream& operator<<(std::ostream& stream, const Pocket::Colour& c) {
            int red = (int)c.r;
            int green = (int)c.r;
            int blue = (int)c.b;
            int alpha = (int)c.a;
            stream<<red<<','<<green<<","<<blue<<","<<alpha;
            return stream;
        }
	};
}
