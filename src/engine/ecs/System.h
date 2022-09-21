#ifndef OPENGL_RENDERER_SYSTEM_H
#define OPENGL_RENDERER_SYSTEM_H

#include "Scene.h"
#include "../../util/Timestep.h"

class System {
public:
    virtual ~System() = default;

    /**
     * Updates the scene based on the behavior of system.
     *
     * @param scene the scene to update
     * @param scene the timestep from the last update
     */
    virtual void update(Scene& scene, const Timestep& ts) = 0;
};


#endif //OPENGL_RENDERER_SYSTEM_H
