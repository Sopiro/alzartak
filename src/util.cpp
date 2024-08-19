#include "alzartak/util.h"

namespace alzartak
{

/*
 * https://gist.github.com/ciembor/1494530
 *
 * Converts an RGB color value to HSL. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes r, g, and b are contained in the set [0, 255] and
 * returns HSL in the set [0, 1].
 */
Vec3 rgb2hsl(float r, float g, float b)
{
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;

    float max = std::max(std::max(r, g), b);
    float min = std::min(std::min(r, g), b);

    Vec3 res{ (max + min) / 2.0f };

    if (max == min)
    {
        // achromatic
        res.x = 0.0f;
        res.y = 0.0f;
    }
    else
    {
        float d = max - min;
        res.x = (res.z > 0.5f) ? d / (2.0f - max - min) : d / (max + min);

        if (max == r)
            res.x = (g - b) / d + (g < b ? 6 : 0);
        else if (max == g)
            res.x = (b - r) / d + 2;
        else if (max == b)
            res.x = (r - g) / d + 4;

        res.x /= 6;
    }

    return res;
}

/*
 * Converts an HUE to r, g or b.
 * returns float in the set [0, 1].
 */
static float hue2rgb(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    else if (t > 1.0f)
        t -= 1.0f;

    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;
    else if (t < 1.0f / 2.0f)
        return q;
    else if (t < 2.0f / 3.0f)
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;

    return p;
}

/*
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns RGB in the set [0, 1].
 */
Vec3 hsl2rgb(float h, float s, float l)
{
    Vec3 res;

    if (s == 0.0f)
    {
        res.x = res.y = res.z = l; // achromatic
    }
    else
    {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        res.x = hue2rgb(p, q, h + 1.0f / 3.0f);
        res.y = hue2rgb(p, q, h);
        res.z = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return res;
}

} // namespace alzartak
