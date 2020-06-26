#pragma once
#include "ray.h"

class camera {
public:
    camera() {}
    camera(vec3 lookfrom, vec3 lookat, vec3 up, float fov, float aspect, float aperture, float focus_dist);
    ray get_ray(float u, float v) const;

private:
    vec3 origin;
    vec3 low_left;
    vec3 horizonal;
    vec3 vertical;
    float lens_radius;
};