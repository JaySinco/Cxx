#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

namespace cxx {

class Camera {
public:
    enum MOVE { FORWARD, BACKWARD, LEFT, RIGHT };
    Camera(const std::string &name, float fov_in_degree, float aspect, float near, float far);
    Camera(const Camera&) = delete;
    ~Camera();
    void move(MOVE direction, float distance);
    void moveTo(float x, float y, float z);
    void shake(float degree);
    void shakeTo(float degree);
    void nod(float degree);
    void nodTo(float degree);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    const std::string id;
private:
    void updateVec();
    const glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 projectionVec = glm::mat4(1.0f);
    glm::vec3 posVec = glm::vec3(0.0f, 0.0f, 0.0f);
    float yawDegree = -90.0f;
    float pitchDegree = 0.0f;
    glm::vec3 frontVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
};

}