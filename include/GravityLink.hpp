#ifndef GRAVITYLINK_HPP
#define GRAVITYLINK_HPP

#include <PMat.hpp>

class GravityLink {
protected:
    PMat* m_PMat1;
    PMat* m_PMat2;

public:
    GravityLink():m_PMat1(nullptr), m_PMat2(nullptr){}
    GravityLink(PMat* pmat1, PMat* pmat2):
        m_PMat1(pmat1),
        m_PMat2(pmat2){

    }

    void setParticles(PMat* pmat1, PMat* pmat2);

    void update();
};

#endif // GRAVITYLINK_HPP
