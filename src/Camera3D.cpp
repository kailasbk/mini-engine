#include "Camera3D.h"

#include <numbers>
#include <glm/ext/matrix_clip_space.hpp>

void Camera3D::moveTo(const glm::vec3& destination) {
    m_position = destination;

    updateViewProjectionMatrix();
}

void Camera3D::lookAt(const glm::vec3& target, bool keepRoll) {
    // get the new direction vector
    m_direction = glm::normalize(m_position - target);

    // update the angles
    if (!keepRoll) {
        m_roll = 0;
    }
    // pitch is in range [-pi/2, pi/2]
    m_pitch = std::asin(-m_direction.y);
    if (m_direction.z > 0.0f) {
        m_yaw = std::asin(-m_direction.x / std::cos(m_pitch));
    } else {
        // yaw is in range [-pi/2, 3pi/2]
        m_yaw = std::numbers::pi_v<float> - std::asin(-m_direction.x / std::cos(m_pitch));
    }
    // TODO: allow the camera to support larger range

    updateViewProjectionMatrix();
}

void Camera3D::rotate(float pitch, float yaw, float roll) {
    // calculate the new angles
    m_pitch += glm::radians(pitch);
    m_yaw += glm::radians(yaw);
    m_roll += glm::radians(roll);
    // TODO: add reset to remove precision loss

    // update the direction vector
    m_direction.x = -std::sin(m_yaw) * std::cos(m_pitch);
    m_direction.y = -std::sin(m_pitch);
    m_direction.z = std::cos(m_pitch) * std::cos(m_yaw);

    updateViewProjectionMatrix();
}

const glm::vec3& Camera3D::getPosition() const {
    return m_position;
}

const glm::vec3& Camera3D::getDirection() const {
    return m_direction;
}

const glm::mat4& Camera3D::getViewProjectionMatrix() const {
    return m_viewProjection;
}

Camera3D::Camera3D(glm::mat4 projection)
        : m_position(0.0f, 0.0f, 0.0f), m_direction(0.0f, 0.0f, 1.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
          m_projection(projection), m_view(), m_viewProjection() {
    updateViewProjectionMatrix();
}

void Camera3D::updateViewProjectionMatrix() {
    // calculate relative x and y axes (direction is z)
    glm::vec3 right(
            std::cos(m_roll) * std::cos(m_yaw),
            -std::sin(m_roll),
            std::cos(m_roll) * std::sin(m_yaw)
    );
    glm::vec3 up = glm::cross(m_direction, right);

    // create the translation matrix
    glm::mat4 translate(1.0f);
    translate[3] = glm::vec4(-m_position, 1.0f);

    // create the basis matrix
    glm::mat4 basis(
            glm::vec4(right, 0.0f),
            glm::vec4(up, 0.0f),
            glm::vec4(m_direction, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    basis = glm::transpose(basis);

    // calculate view and view projection matrices
    m_view = basis * translate;
    m_viewProjection = m_projection * m_view;
}

std::shared_ptr<Camera3D> Camera3D::makeOrthographic(float width, float height, float zNear, float zFar) {
    glm::mat4 projection = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
    return std::shared_ptr<Camera3D>(new Camera3D(projection));
}

std::shared_ptr<Camera3D> Camera3D::makePerspective(float fieldOfView, float aspectRatio, float zNear, float zFar) {
    glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, zNear, zFar);
    return std::shared_ptr<Camera3D>(new Camera3D(projection));
}
