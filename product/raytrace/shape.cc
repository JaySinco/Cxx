#include "shape.h"

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    bool hit_anything = false;
    float closest_so_far = t_max;
    for (const auto& it : list) {
        hit_record tmp_rec;
        if (it->hit(r, t_min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }
    return hit_anything;
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
    float a = r.dir().dot(r.dir());
    float b = oc.dot(r.dir());
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float t = (-b - std::sqrt(discriminant)) / a;
        if (t >= t_max || t <= t_min) {
            t = (-b + std::sqrt(discriminant)) / a;
        }
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
