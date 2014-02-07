#ifndef GRAVITYLINK_HPP
#define GRAVITYLINK_HPP

#include <PMat.hpp>

class GravityLink {
protected:
    PMat* m_PMat1;
    PMat* m_PMat2;
    float m_G;
    float m_distRatio;

public:
    GravityLink():
        m_PMat1(nullptr),
        m_PMat2(nullptr),
        m_G(0.000000000066742f),
        m_distRatio(1){

    }

    GravityLink(const double G):
        m_PMat1(nullptr),
        m_PMat2(nullptr),
        m_G(G),
        m_distRatio(1){

    }

    GravityLink(const double G, const int ratio):
        m_PMat1(nullptr),
        m_PMat2(nullptr),
        m_G(G),
        m_distRatio(ratio){

    }

    void setParticles(PMat* pmat1, PMat* pmat2);

    float& getGravityForce(){
        return m_G;
    }

    float& getDistRatio(){
        return m_distRatio;
    }

    void update();
};

#endif // GRAVITYLINK_HPP
