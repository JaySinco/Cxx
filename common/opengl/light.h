#pragma once
#include "base.h"

namespace cxx {

namespace gl {

class Light: public Base {
public:
    enum Type { DIRECT=0, POINT=1, SPOT=2, GLOBAL=3 };
    struct Data {
        Type all_type;
        glm::vec3 all_ambient;
        glm::vec3 all_diffuse;
        glm::vec3 all_specular;
        glm::vec3 direct_spot_direction;
        glm::vec3 point_spot_position;
        float point_constant;
        float point_linear;
        float point_quadratic;
        float spot_innerCutOff;
        float spot_outerCutOff;
    };
    // global
    Light(const std::string &id, glm::vec3 ambient): Base(Base::LIGHT, id) {
        data_.all_type = GLOBAL;
        data_.all_ambient = ambient;
    }
    // directional
    Light(
            const std::string &id,
            glm::vec3 direction,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular): Base(Base::LIGHT, id) {

        data_.all_type = DIRECT;
        data_.direct_spot_direction = direction;
        data_.all_ambient = ambient;
        data_.all_diffuse = diffuse;
        data_.all_specular = specular;
    }
    // point
    Light(
            const std::string &id,
            glm::vec3 position,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            float constant,
            float linear,
            float quadratic): Base(Base::LIGHT, id) {

        data_.all_type = POINT;
        data_.all_ambient = ambient;
        data_.all_diffuse = diffuse;
        data_.all_specular = specular;
        data_.point_spot_position = position;
        data_.point_constant = constant;
        data_.point_linear = linear;
        data_.point_quadratic = quadratic;
    }
    // spot
    Light(
            const std::string &id,
            glm::vec3 position,
            glm::vec3 direction,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            float innerDegree,
            float outerDegree): Base(Base::LIGHT, id) {

        data_.all_type = SPOT;
        data_.all_ambient = ambient;
        data_.all_diffuse = diffuse;
        data_.all_specular = specular;
        data_.point_spot_position = position;
        data_.direct_spot_direction = direction;
        data_.spot_innerCutOff = glm::cos(glm::radians(innerDegree));
        data_.spot_outerCutOff = glm::cos(glm::radians(outerDegree));
    }
    Data data() const { return data_; }
private:
    Data data_;
};

} // namespace gl

} // namespace cxx