#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <PMat.hpp>

class Particle : public PMat{
public:
    Particle():PMat(){}
    Particle(const glm::vec3& position):PMat(position){}
    Particle(const glm::vec3& position, const float mass):PMat(position, mass){}
    Particle(const glm::vec3& position, const glm::vec3& velocity, const float mass):PMat(position, velocity, mass){}

    void update();

};

#endif // PARTICLE_HPP
