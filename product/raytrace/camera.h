#pragma once
#include "ray.h"

class camera {
public:
    ray get_ray(float u, float v) const { return { origin, low_left + u * horizonal + v * vertical }; }

private:
    vec3 origin { 0, 0, 0 };
    vec3 low_left { -2, -1, -1 };
    vec3 horizonal { 4, 0, 0 };
    vec3 vertical { 0, 2, 0 };
};