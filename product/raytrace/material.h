#pragma once
#include "ray.h"
#include <algorithm>

class diffuse : public material {
public:
    diffuse(const vec3& a)
        : albedo(a)
    {
    }
    bool scatter(const ray& r_in, const hit_record& rec, vec3& fade, ray& r_out) const override;

private:
    vec3 albedo;
};

class metal : public material {
public:
    metal(const vec3& a, float f)
        : albedo(a)
        , fuzz(std::min(f, 1.0f))
    {
    }
    bool scatter(const ray& r_in, const hit_record& rec, vec3& fade, ray& r_out) const override;

private:
    vec3 albedo;
    float fuzz;
};
