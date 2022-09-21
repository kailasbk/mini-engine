#include "Camera3D.h"

#include "../util/angle.h"

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
        m_yaw = PI - std::asin(-m_direction.x / std::cos(m_pitch));
    }
    // TODO: allow the camera to support larger range

    updateViewProjectionMatrix();
}

void Camera3D::rotate(float pitch, float yaw, float roll) {
    // calculate the new angles
    m_pitch += degreesToRadians(pitch);
    m_yaw += degreesToRadians(yaw);
    m_roll += degreesToRadians(roll);
    // TODO: add reset to remove precision loss

    // update the direction vector
    m_direction.x = -std::sin(m_yaw) * std::cos(m_pitch);
    m_direction.y = -std::sin(m_pitch);
    m_direction.z = std::cos(m_pitch) * std::cos(m_yaw);

    updateViewProjectionMatrix();
}

Camera3D::Camera3D(Mat4 projection)
        : m_position(0.0f, 0.0f, 0.0f), m_direction(0.0f, 0.0f, 1.0f), m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
          m_projection(projection), m_view(), m_viewProjection() {
    updateViewProjectionMatrix();
}

void Camera3D::updateViewProjectionMatrix() {
    // calculate relative x and y axes (direction is z)
    Vec3 right(std::cos(m_roll) * std::cos(m_yaw), -std::sin(m_roll), std::cos(m_roll) * std::sin(m_yaw));
    Vec3 up = m_direction.cross(right);

    // create the translation matrix
    Mat4 translate({
        Vec4(1.0f, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, 1.0f, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, 1.0f, 0.0f),
        Vec4(-m_position.x, -m_position.y, -m_position.z, 1.0f)
    });

    // create the basis matrix
    Mat4 basis({
        Vec4(right.x, up.x, m_direction.x, 0.0f),
        Vec4(right.y, up.y, m_direction.y, 0.0f),
        Vec4(right.z, up.z, m_direction.z, 0.0f),
        Vec4(0.0f, 0.0f, 0.0f, 1.0f)
    });

    // calculate view and view projection matrices
    m_view = basis * translate;
    m_viewProjection = m_projection * m_view;
}

std::shared_ptr<Camera3D> Camera3D::createOrthographic(float width, float height, float zNear, float zFar) {
    // TODO: seems bugged, need to fix
    Mat4 projection({
        Vec4(2.0f / width, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, 2.0f / height, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, -2.0f / (zFar - zNear), 0.0f),
        Vec4(0.0f, 0.0f, -(zNear + zFar) / 2.0f, 1.0f),
    });
    return std::make_shared<Camera3D>(projection);
}

std::shared_ptr<Camera3D> Camera3D::createPerspective(float fieldOfView, float aspectRatio, float zNear, float zFar) {
    // projection[0,0] = zNear / (aspect * zNear * (sin/cos))
    // projection[1,1] = zNear / (zNear * (sin/cos))
    float y = std::cos(fieldOfView / 2.0f) / std::sin(fieldOfView / 2.0f);
    float x = y / aspectRatio;
    Mat4 projection({
        Vec4(x, 0.0f, 0.0f, 0.0f),
        Vec4(0.0f, y, 0.0f, 0.0f),
        Vec4(0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f),
        Vec4(0.0f, 0.0f, -2.0f * zFar * zNear / (zFar - zNear), 0.0f),
    });
    return std::make_shared<Camera3D>(projection);
}
