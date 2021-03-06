#ifndef NBODYSIM_HPP
#define NBODYSIM_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <tr1/memory>
#include <vector>

#include <FreeFlyCamera.hpp>
#include <MatrixStack.hpp>
#include <Circle.hpp>
#include <GLProgram.hpp>
#include <Particle.hpp>
#include <FixedPoint.hpp>
#include <GravityLink.hpp>
#include <GUI.hpp>

class NBodySim {
private:
    GLFWwindow* m_window;

    GravityLink m_gravity;
    std::vector<std::tr1::shared_ptr<PMat>> m_particles;

    GLProgram m_program;
    FreeFlyCamera m_camera;
    MatrixStack m_stack;
    GLuint m_MVPLocation;
    GLuint m_colorLocation;
    Circle m_circle;
    GUI m_gui;

    glm::vec3 m_planetInitVelocity;
    glm::vec3 m_satelliteInitVelocity;

    float m_fps;
    bool m_launched;

    void initWindow(const int WIDTH, const int HEIGHT);
    void initPhysics();
    void initGraphics();
    void terminate();

public:
    NBodySim();
    void init(const int WIDTH, const int HEIGHT);
    void run();
    void reset();
    void start();
};

#endif // NBODYSIM_HPP
