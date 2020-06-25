#include "ray.h"
#include <random>

float rand0_1()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0, 1);
    return dis(gen);
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do {
        p = 2 * vec3(rand0_1(), rand0_1(), rand0_1()) - vec3(1, 1, 1);
    } while (p.dot(p) >= 1);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * v.dot(n) * n;
}
