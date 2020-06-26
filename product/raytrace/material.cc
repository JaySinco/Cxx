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

bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& decay, ray& r_out) const
{
    vec3 reflected = reflect(normalize(r_in.dir()), rec.n);
    decay = vec3(1, 1, 1);
    float ni_over_nt, cosine, reflect_prob;
    vec3 outward_normal;
    if (dot(r_in.dir(), rec.n) > 0) {
        outward_normal = -rec.n;
        ni_over_nt = ref_idx;
        cosine = ref_idx * dot(r_in.dir(), rec.n) / length(r_in.dir());
    } else {
        outward_normal = rec.n;
        ni_over_nt = 1 / ref_idx;
        cosine = -1 * dot(r_in.dir(), rec.n) / length(r_in.dir());
    }
    vec3 refracted;
    if (refract(r_in.dir(), outward_normal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cosine, ref_idx);
    } else {
        reflect_prob = 1.0;
    }
    if (rand0_1() < reflect_prob) {
        r_out = ray(rec.p, reflected);
    } else {
        r_out = ray(rec.p, refracted);
    }
    return true;
}