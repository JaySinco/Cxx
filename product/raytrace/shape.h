#pragma once
#include "material.h"
#include "ray.h"
#include <vector>

class hitable_list : public hitable {
public:
    hitable_list() {}
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    template <typename T, typename... Args>
    void put(Args&&... args)
    {
        list.push_back(std::make_shared<T>(std::forward<Args>(args)...));
    }

private:
    std::vector<std::shared_ptr<hitable>> list;
};

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 c, float r, std::shared_ptr<material> m)
        : center(c)
        , radius(r)
        , mat(m)
    {
    }
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
    vec3 center;
    float radius;
    std::shared_ptr<material> mat;
};
