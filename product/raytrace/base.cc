#include "base.h"
#include <random>

float rand0_1()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0, 1);
    return dis(gen);
}

float dot(const vec3& v1, const vec3& v2)
{
    return v1.dot(v2);
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do {
        p = 2 * vec3(rand0_1(), rand0_1(), rand0_1()) - vec3(1, 1, 1);
    } while (dot(p, p) >= 1);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

vec3 normalize(const vec3& v)
{
    return cv::normalize(v);
}

vec3 mul(const vec3& v1, const vec3& v2)
{
    return v1.mul(v2);
}

vec3 sqrt(const vec3& v)
{
    vec3 res;
    res[0] = std::sqrt(v[0]);
    res[1] = std::sqrt(v[1]);
    res[2] = std::sqrt(v[2]);
    return res;
}