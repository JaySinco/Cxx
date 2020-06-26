#include "camera.h"

camera::camera(vec3 lookfrom, vec3 lookat, vec3 up, float fov, float aspect, float aperture, float focus_dist)
{
    float theta = fov * M_PI / 180;
    float half_height = std::tan(theta / 2);
    float half_width = aspect * half_height;
    vec3 w = normalize(lookfrom - lookat);
    vec3 u = normalize(cross(up, w));
    vec3 v = cross(w, u);
    origin = lookfrom;
    low_left = origin - focus_dist * (half_width * u + half_height * v + w);
    horizonal = 2 * focus_dist * half_width * u;
    vertical = 2 * focus_dist * half_height * v;
    lens_radius = aperture / 2;
}

ray camera::get_ray(float u, float v) const
{
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd[0] + v * rd[1];
    return { origin + offset, low_left + u * horizonal + v * vertical - origin - offset };
}