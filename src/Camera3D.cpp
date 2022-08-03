#include "Camera3D.h"

#include <numbers>
#include <glm/ext/matrix_clip_space.hpp>

void Camera3D::moveTo(const Vec3& destination) {
    m_position = destination;

    updateViewProjectionMatrix();
}

void Camera3D::lookAt(const Vec3& target, bool keepRoll) {
    // get the new normalized direction vector
    m_direction = m_position - target;
    m_direction = m_direction * (1 / std::sqrtf(m_direction.dot(m_direction)));

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

Camera3D::Camera3D(glm::mat4 proj)
        : m_position(0.0f, 0.0f, 0.0f), m_direction(0.0f, 0.0f, 1.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
          m_projection({Vec4(proj[0][0], proj[0][1], proj[0][2], proj[0][3]),
                        Vec4(proj[1][0], proj[1][1], proj[1][2], proj[1][3]),
                        Vec4(proj[2][0], proj[2][1], proj[2][2], proj[2][3]),
                        Vec4(proj[3][0], proj[3][1], proj[3][2], proj[3][3])}),
          m_view(), m_viewProjection() {
    updateViewProjectionMatrix();
}

void Camera3D::updateViewProjectionMatrix() {
    // calculate relative x and y axes (direction is z)
    glm::vec3 right(
            std::cos(m_roll) * std::cos(m_yaw),
            -std::sin(m_roll),
            std::cos(m_roll) * std::sin(m_yaw)
    );

    glm::vec3 up = glm::cross({m_direction.x, m_direction.y, m_direction.z}, right);

    // create the translation matrix
    Mat4 translate({
            Vec4(1.0f, 0.0f, 0.0f, 0.0f),
            Vec4(0.0f, 1.0f, 0.0f, 0.0f),
            Vec4(0.0f, 0.0f, 1.0f, 0.0f),
            Vec4(-m_position.x, -m_position.y, -m_position.z, 1.0f)
    });

    // create the basis matrix
    Mat4 basis = Mat4({
            Vec4(right.x, up.x, m_direction.x, 0.0f),
            Vec4(right.y, up.y, m_direction.y, 0.0f),
            Vec4(right.z, up.z, m_direction.z, 0.0f),
            Vec4(0.0f, 0.0f, 0.0f, 1.0f)
    });

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
