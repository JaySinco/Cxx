#pragma once
#include <opencv2/core/matx.hpp>

typedef cv::Vec3f vec3;

class ray {
public:
    ray() {}
    ray(const vec3& a, const vec3& b)
        : A(a)
        , B(b)
    {
    }
    vec3 origin() const { return A; }
    vec3 dir() const { return B; }
    vec3 point_at_parameter(float t) const { return A + t * B; }

private:
    vec3 A;
    vec3 B;
};

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};