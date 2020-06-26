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

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = normalize(v);
    float dt = dot(uv, n);
    float discrim = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discrim > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * std::sqrt(discrim);
        return true;
    } else {
        return false;
    }
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}

float length(const vec3& v)
{
    return std::sqrt(dot(v, v));
}