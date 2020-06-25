#include "material.h"

bool diffuse::scatter(const ray& r_in, const hit_record& rec, vec3& fade, ray& r_out) const
{
    r_out = ray(rec.p, rec.normal + random_in_unit_sphere());
    fade = albedo;
    return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& fade, ray& r_out) const
{
    vec3 reflected = reflect(cv::normalize(r_in.dir()), rec.normal);
    r_out = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    fade = albedo;
    return (r_out.dir().dot(rec.normal) > 0);
}