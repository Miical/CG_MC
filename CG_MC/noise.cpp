#include <math.h>
#include "setting.h"
#include "noise.h"

/// <summary>
/// 一维插值函数。
/// 使用缓和曲线 s(t) = 6t^5 − 15t^4 + 10t
/// </summary>
/// <param name="pos">需要插值的位置，范围在0-1之间</param>
/// <param name="v">两个端点值数据</param>
/// <returns>插值结果</returns>
float interpolate1D(float pos, float v[2]) {
    float s = pos * pos * pos * (pos * (pos * 6.0f - 15.0f) + 10.0f);
    return (v[1] - v[0]) * s + v[0];
}

/// <summary>
/// 二维插值函数。
/// 使用缓和曲线 s(t) = 6t^5 − 15t^4 + 10t
/// </summary>
/// <param name="v">四个端点的数据，
///     [(x0, y0), (x0 + 1, y0), (x0, y0 + 1), (x0 + 1, y0 +1)]</param>
/// <returns>插值结果</returns>
float interpolate2D(Point2D pos, float v[4]) {
    float pv[2];
    pv[0] = v[0]; pv[1] = v[1];
    float x0 = interpolate1D(pos.x, pv);
    pv[0] = v[2]; pv[1] = v[3];
    float x1 = interpolate1D(pos.x, pv);
    pv[0] = x0; pv[1] = x1;
    return interpolate1D(pos.y, pv);
}

/// <summary>
/// 由坐标值生成随机单位向量。
/// </summary>
/// <returns>随机单位向量</returns>
Vector2D randomGradient2D(Point2D pos, uint seed) {
    const uint w = 8 * sizeof(uint);
    const uint s = w / 2;
    uint a = uint(pos.x), b = uint(pos.y);
    a *= 3284157443; a ^= seed; b ^= a << s | a >> w - s;
    b *= 1911520717; b ^= seed; a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    return Vector2D(cosf(random), sinf(random));
}

/// <summary>
/// 柏林噪声函数。
/// </summary>
/// <param name="pos">位置</param>
/// <returns>噪声值，范围在0-1内</returns>
float perlin(Point2D pos, uint seed) {
    int x0 = (int)floor(pos.x), y0 = (int)floor(pos.y);
    Point2D pv[4] = {
        Point2D(x0, y0), Point2D(x0 + 1, y0),
        Point2D(x0, y0 + 1), Point2D(x0 + 1, y0 + 1)
    };
    float v[4] = {
        Dot(pos - pv[0], randomGradient2D(pv[0], seed)),
        Dot(pos - pv[1], randomGradient2D(pv[1], seed)),
        Dot(pos - pv[2], randomGradient2D(pv[2], seed)),
        Dot(pos - pv[3], randomGradient2D(pv[3], seed)),
    };
    float value = interpolate2D(
        Point2D(pos.x - x0, pos.y - y0), v);
    return (value + 1.0f) / 2.0f;
}
