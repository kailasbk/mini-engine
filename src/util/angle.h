#ifndef ENGINE_ANGLE_H
#define ENGINE_ANGLE_H

#include <numbers>

constexpr float PI = std::numbers::pi_v<float>;

/**
 * Converts from degrees to radians.
 *
 * @param degrees the angle in degrees
 * @returns the same angle in radians
 */
inline float degreesToRadians(float degrees) {
    return degrees * PI / 180.0f;
}

#endif //ENGINE_ANGLE_H
