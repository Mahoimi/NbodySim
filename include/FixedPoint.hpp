#ifndef FIXEDPOINT_HPP
#define FIXEDPOINT_HPP

#include <PMat.hpp>

class FixedPoint : public PMat{
public:
    FixedPoint():PMat(){}
    FixedPoint(const glm::vec3& position):PMat(position){}
    FixedPoint(const glm::vec3& position, const float mass):PMat(position, mass){}

    void update();
};

#endif // FIXEDPOINT_HPP
