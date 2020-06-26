#pragma once
#include "shape.h"
#include <algorithm>

class material {
public:
    virtual ~material() {};
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const = 0;
};

class diffuse : public material {
public:
    diffuse(const vec3& albedo)
        : albedo_(albedo)
    {
    }
    ~diffuse() = default;
    bool scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const override;

private:
    vec3 albedo_;
};

class metal : public material {
public:
    metal(const vec3& albedo, float fuzz)
        : albedo_(albedo)
        , fuzz_(std::min(fuzz, 1.0f))
    {
    }
    ~metal() = default;
    bool scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const override;

private:
    float fuzz_;
    vec3 albedo_;
};

class dielectric : public material {
public:
    dielectric(float ri)
        : ref_idx(ri)
    {
    }
    ~dielectric() = default;
    bool scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const override;

private:
    float ref_idx;
};
