#ifndef OPENGL_RENDERER_TIMESTEP_H
#define OPENGL_RENDERER_TIMESTEP_H

#include <chrono>

using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

using Duration = std::chrono::duration<double>;

struct Timestep {
public:
    Duration deltaT; // interval in seconds
    Timestamp t; // absolute timestamp from clock

    /**
     * Generates a timestep to be used to start a chain of subsequent timesteps.
     *
     * @returns the timestep
     */
    static Timestep start() {
        Timestamp t = std::chrono::steady_clock::now();
        return Timestep{
            .deltaT = Duration::zero(),
            .t = t,
        };
    }

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
};


#endif //OPENGL_RENDERER_TIMESTEP_H
