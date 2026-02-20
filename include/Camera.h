#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    void setPerspective(float fov, float aspect, float near, float far) {
        _projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
        _view = glm::lookAt(eye, center, up);
    }

    const glm::mat4& projection() const { return _projection; }
    const glm::mat4& view() const { return _view; }

private:
    glm::mat4 _projection;
    glm::mat4 _view;
};
