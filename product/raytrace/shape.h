#pragma once
#include "ray.h"
#include <vector>

class material;

struct hit_record {
    float t; // distance by ray direction
    vec3 p; // hit point
    vec3 n; // normal of hit surface
    std::shared_ptr<material> m; // material of hit surface
};

class shape {
public:
    virtual ~shape() {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;

    template <typename T, typename... Args>
    void made_of(Args&&... args)
    {
        material_ = std::make_shared<T>(std::forward<Args>(args)...);
    }

    std::shared_ptr<material> made_from() { return material_; }

protected:
    std::shared_ptr<material> material_;
};

class container : public shape {
public:
    ~container() = default;
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    void put(std::shared_ptr<shape> obj) { list.push_back(obj); }

private:
    std::vector<std::shared_ptr<shape>> list;
};

class sphere : public shape {
public:
    sphere(vec3 center, float radius)
        : center_(center)
        , radius_(radius)
    {
    }
    ~sphere() = default;
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
    vec3 center_;
    float radius_;
};
