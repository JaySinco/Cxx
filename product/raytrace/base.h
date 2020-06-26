#pragma once
#include "common/utility/base.h"
#include <memory>
#include <opencv2/core/core.hpp>

#define RGB(r, g, b) vec3(b, g, r)

typedef cv::Vec3f vec3;

template <typename T, typename... Args>
std::shared_ptr<T> create(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

float rand0_1();
float dot(const vec3& v1, const vec3& v2);
float length(const vec3& v);
vec3 random_in_unit_sphere();
vec3 random_in_unit_disk();
vec3 sqrt(const vec3& v);
vec3 normalize(const vec3& v);
vec3 mul(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 reflect(const vec3& v, const vec3& n);
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted);
float schlick(float cosine, float ref_idx);