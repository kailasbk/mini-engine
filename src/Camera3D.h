#ifndef OPENGL_RENDERER_CAMERA3D_H
#define OPENGL_RENDERER_CAMERA3D_H


class Camera3D {
public:
    /**
     * Constructs a orthographic camera with the given viewport width and height and
     * the given near and far planes.
     *
     * @param width the width of the viewport
     * @param height the height of the viewport
     * @param zNear the z coordinate of the near plane
     * @param zFar the z coordinate of the far plane
     */
    static std::shared_ptr<Camera3D> makeOrthographic(float width, float height, float zNear, float zFar);

    /**
     * Constructs a perspective camera with the given viewport field of view and aspect ratio
     * and the given near and far planes.
     *
     * @param fieldOfView the field of view of the viewport, in degrees
     * @param aspectRatio the aspect ratio of the viewport
     * @param zNear the z coordinate of the near plane
     * @param zFar the z coordinate of the far plane
     */
    static std::shared_ptr<Camera3D> makePerspective(float fieldOfView, float aspectRatio, float zNear, float zFar);

    /**
     * Moves the camera to the given position, without rotation.
     *
     * @param destination the position to move the camera to
     */
    virtual void moveTo(const glm::vec3& destination);

    /**
     * Rotates the camera to look at the target position while keeping the roll the same
     * if specified, otherwise resetting it to zero.
     *
     * @param target the position for the camera to look at
     * @param keepRoll whether to keep the roll
     */
    virtual void lookAt(const glm::vec3& target, bool keepRoll);

    /**
     * Rotates the camera by the given pitch, yaw and roll from its current direction.
     *
     * @param pitch the pitch angle change, in degrees
     * @param yaw the yaw angle change, in degrees
     * @param roll the roll angle change, in degrees
     */
    virtual void rotate(float pitch, float yaw, float roll);

    /**
     * @return the position of the camera
     */
    virtual const glm::vec3& getPosition() const;

    /**
     * @return the unit vector in the direction the camera is pointing
     */
    virtual const glm::vec3& getDirection() const;

    /**
     * @return the view projection (view then projection transform) matrix of the camera
     */
    virtual const glm::mat4& getViewProjectionMatrix() const;
protected:
    /**
     * Constructs the camera with the given projection matrix.
     */
    explicit Camera3D(glm::mat4 projection);

    /**
     * Given the position, direction, and roll of the camera, update the
     * view projection matrix.
     */
    void updateViewProjectionMatrix();

    glm::vec3 m_position;
    glm::vec3 m_direction;
    float m_pitch, m_yaw, m_roll; // in radians
    glm::mat4 m_view;
    const glm::mat4 m_projection;
    glm::mat4 m_viewProjection;
};


#endif //OPENGL_RENDERER_CAMERA3D_H
