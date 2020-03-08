#pragma once
#include "base.h"
#include <vector>

namespace cxx {

namespace gl {

    class Camera : public Base {
    public:
        enum MOVE {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };
        Camera(
            const std::string& id,
            float fov_in_degree,
            float aspect,
            float near,
            float far);
        void move(MOVE direction, float distance);
        void moveTo(float x, float y, float z);
        void shake(float degree);
        void shakeTo(float degree);
        void nod(float degree);
        void nodTo(float degree);
        glm::vec3 pos() const;
        glm::vec3 front() const;
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix() const;

    private:
        void updateVec();
        glm::vec3 posVec = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 projectionVec = glm::mat4(1.0f);
        float yawDegree = -90.0f;
        float pitchDegree = 0.0f;
        glm::vec3 frontVec;
        glm::vec3 upVec;
        glm::vec3 rightVec;
    };

} // namespace gl

} // namespace cxx