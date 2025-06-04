#pragma once

#include "common.h"

namespace alzartak
{

// https://en.wikipedia.org/wiki/SRGB
inline Vec3 RGB_from_sRGB(const Vec3& sRGB)
{
    Vec3 RGB;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = sRGB[i];

        if (comp <= 0.04045f)
        {
            RGB[i] = comp * (1.0f / 12.92f);
        }
        else
        {
            RGB[i] = std::pow((comp + 0.055f) * (1.0f / 1.055f), 2.4f);
        }
    }

    return RGB;
}

inline Vec3 sRGB_from_RGB(const Vec3& RGB)
{
    Vec3 sRGB;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = RGB[i];

        if (comp <= 0.0031308f)
        {
            sRGB[i] = 12.92f * comp;
        }
        else
        {
            sRGB[i] = (1.0f + 0.055f) * std::pow(comp, 1.0f / 2.4f) - 0.055f;
        }
    }

    return sRGB;
}

inline Point3 HSLtoRGB(Float h, Float s, Float l)
{
    Float r, g, b;

    const static auto hue2rgb = [](Float p, Float q, Float t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    if (s == 0.0f)
    {
        r = g = b = l;
    }
    else
    {
        Float q = l < 0.5f ? (l * (1.0f + s)) : (l + s - l * s);
        Float p = 2.0f * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return Point3(r, g, b);
}

inline Point3 RGBtoHSL(Float r, Float g, Float b)
{
    Float max = std::fmax(r, std::fmax(g, b));
    Float min = std::fmin(r, std::fmin(g, b));
    Float h, s, l;

    l = (max + min) * 0.5f;

    if (max == min)
    {
        h = 0.0f;
        s = 0.0f;
    }
    else
    {
        Float d = max - min;
        s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);

        if (max == r)
        {
            h = (g - b) / d + (g < b ? 6.0f : 0.0f);
        }
        else if (max == g)
        {
            h = (b - r) / d + 2.0f;
        }
        else
        { // max == b
            h = (r - g) / d + 4.0f;
        }

        h /= 6.0f;
    }

    return Point3(h, s, l);
}

// https://www.shadertoy.com/view/WdjSW3
inline Vec3 Tonemap_Reinhard(const Vec3& RGB)
{
    return RGB / (Vec3(1.0) + RGB);
}

inline Vec3 Tonemap_Reinhard2(const Vec3& RGB)
{
    const Vec3 L_white{ 4 };
    return (RGB * (Vec3(1.0) + RGB / (L_white * L_white))) / (Vec3(1.0) + RGB);
}

inline Vec3 Tonemap_ACES(const Vec3& RGB)
{
    const Float a = 2.51f;
    const Float b = 0.03f;
    const Float c = 2.43f;
    const Float d = 0.59f;
    const Float e = 0.14f;
    return (RGB * (a * RGB + Vec3(b))) / (RGB * (c * RGB + Vec3(d)) + Vec3(e));
}

// See below for more:
// https://www.khronos.org/news/press/khronos-pbr-neutral-tone-mapper-released-for-true-to-life-color-rendering-of-3d-products
inline Vec3 ToneMap_PBRNeutral(Vec3 RGB)
{
    const Float startCompression = 0.8f - 0.04f;
    const Float desaturation = 0.15f;

    Float x = std::min(RGB.x, std::min(RGB.y, RGB.z));
    Float offset = x < 0.08f ? x - 6.25f * x * x : 0.04f;
    RGB -= Vec3(offset);

    Float peak = std::max(RGB.x, std::max(RGB.y, RGB.z));
    if (peak < startCompression) return RGB;

    const Float d = 1.f - startCompression;
    Float newPeak = 1.f - d * d / (peak + d - startCompression);
    RGB *= newPeak / peak;

    Float g = 1.f - 1.f / (desaturation * (peak - newPeak) + 1.f);
    return Lerp(RGB, newPeak * Vec3(1, 1, 1), g);
}

} // namespace alzartak
