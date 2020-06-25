#pragma once
#include <memory>
#include <opencv2/core/matx.hpp>

#define RGB(r, g, b) vec3(b, g, r)

typedef cv::Vec3f vec3;

template <typename T, typename... Args>
std::shared_ptr<T> create(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

float rand0_1();
float dot(const vec3& v1, const vec3& v2);
vec3 random_in_unit_sphere();
vec3 sqrt(const vec3& v);
vec3 normalize(const vec3& v);
vec3 mul(const vec3& v1, const vec3& v2);
vec3 reflect(const vec3& v, const vec3& n);
