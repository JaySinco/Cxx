#pragma once
#include "base.h"

class ray {
public:
    ray() {}
    ray(const vec3& origin, const vec3& direction)
        : origin_(origin)
        , dir_(direction)
    {
    }
    vec3 origin() const { return origin_; }
    vec3 dir() const { return dir_; }
    vec3 endpoint(float t) const { return origin_ + t * dir_; }

private:
    vec3 origin_;
    vec3 dir_;
};
