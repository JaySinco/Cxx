#include "shape.h"

bool container::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    bool hit_anything = false;
    float closest_so_far = t_max;
    for (const auto& it : list) {
        hit_record it_rec;
        if (it->hit(r, t_min, closest_so_far, it_rec)) {
            hit_anything = true;
            closest_so_far = it_rec.t;
            rec = it_rec;
        }
    }
    return hit_anything;
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center_;
    float a = dot(r.dir(), r.dir());
    float b = dot(oc, r.dir());
    float c = dot(oc, oc) - radius_ * radius_;
    float discrim = b * b - a * c;
    if (discrim > 0) {
        float t = (-b - std::sqrt(discrim)) / a;
        if (t >= t_max || t <= t_min) {
            t = (-b + std::sqrt(discrim)) / a;
        }
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.endpoint(t);
            rec.n = (rec.p - center_) / radius_;
            rec.m = material_;
            return true;
        }
    }
    return false;
}
