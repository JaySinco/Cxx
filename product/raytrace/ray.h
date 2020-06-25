#pragma once
#include <memory>
#include <opencv2/core/matx.hpp>

typedef cv::Vec3f vec3;

float rand0_1();
vec3 random_in_unit_sphere();
vec3 reflect(const vec3& v, const vec3& n);

class ray {
public:
    ray() {}
    ray(const vec3& origin, const vec3& direction)
        : A(origin)
        , B(direction)
    {
    }
    vec3 origin() const { return A; }
    vec3 dir() const { return B; }
    vec3 at(float t) const { return A + t * B; }

private:
    vec3 A;
    vec3 B;
};

class material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
};

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& fade, ray& r_out) const = 0;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};