#include <Particle.hpp>

const float H = 1.f;

void Particle::update(){
    // Leapfrog solver
    m_velocity += m_force * (H / m_mass);
    m_position += m_velocity * H;
    m_force = glm::vec3(0);
}
