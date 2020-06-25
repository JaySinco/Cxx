#include "material.h"

bool diffuse::scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const
{
    r_out = ray(rec.p, rec.n + random_in_unit_sphere());
    decay = albedo_;
    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const
{
    vec3 reflected = reflect(normalize(r_in.dir()), rec.n);
    r_out = ray(rec.p, reflected + fuzz_ * random_in_unit_sphere());
    decay = albedo_;
    return dot(r_out.dir(), rec.n) > 0;
}