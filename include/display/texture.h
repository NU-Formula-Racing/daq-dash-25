#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdint.h>

/// @brief A compact representation of a color
/// @details A color is represented by 4 bytes, one for each channel
struct Color {
   public:
    uint8_t r, g, b, a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
    Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

    static Color fromFloat(float r, float g, float b, float a = 1.0f) {
        return Color(
            Color::toChar(r),
            Color::toChar(g),
            Color::toChar(b),
            Color::toChar(a));
    }

    static Color greyscale(float v) {
        return Color::fromFloat(v, v, v);
    }

    static Color lerp(const Color &a, const Color &b, float t) {
        return (1 - t) * a + t * b;
    }

    Color &operator=(const Color &c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    Color operator+(const Color &c) const {
        return Color(r + c.r, g + c.g, b + c.b, a + c.a);
    }

    Color operator-(const Color &c) const {
        return Color(r - c.r, g - c.g, b - c.b, a - c.a);
    }

    Color operator*(const Color &c) const {
        // Convert to float, to avoid overflow
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        float cr = c.toFloat(c.r);
        float cg = c.toFloat(c.g);
        float cb = c.toFloat(c.b);
        float ca = c.toFloat(c.a);

        return Color::fromFloat(fr * cr, fg * cg, fb * cb, fa * ca);
    }

    Color operator*(float f) const {
        // Convert to float, to avoid overflow
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);
        return Color::fromFloat(fr * f, fg * f, fb * f, fa * f);
    }

    float getLuminance() const {
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        float l = 0.2126f * fr + 0.7152f * fg + 0.0722f * fb;

        return l;
    }

    std::string toString() const {
        float fr = this->toFloat(r);
        float fg = this->toFloat(g);
        float fb = this->toFloat(b);
        float fa = this->toFloat(a);

        std::stringstream ss;
        ss << "(" << fr << ", " << fg << ", " << fb << ", " << fa << ")";
        return ss.str();
    }

   private:
    /// @brief converts 0-255 to 0-1 (char to float)
    static inline float toFloat(uint8_t c) {
        return (float)c / 255.0f;
    }

    /// @brief converts 0-1 to 0-255 (float to char)
    static inline uint8_t toChar(float f) {
        return (uint8_t)(f * 255.0f);
    }
};

#endif  // __TEXTURE_H__