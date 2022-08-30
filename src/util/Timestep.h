#ifndef OPENGL_RENDERER_TIMESTEP_H
#define OPENGL_RENDERER_TIMESTEP_H

#include <chrono>

using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

using Duration = std::chrono::duration<double>;

struct Timestep {
public:
    const Duration deltaT; // interval in seconds
    const Timestamp t; // absolute timestamp from clock

    /**
     * Generates a timestep that is the measured from given the previous timestep to the current time.
     *
     * @param previous the previous timestep
     * @returns the timestep representing the interval between the previous one and now
     */
    static Timestep now(Timestep previous) {
        Timestamp t = std::chrono::steady_clock::now();
        return Timestep{
            .deltaT = t - previous.t,
            .t = t,
        };
    }

private:
    static const Timestamp t_0;
};


#endif //OPENGL_RENDERER_TIMESTEP_H
