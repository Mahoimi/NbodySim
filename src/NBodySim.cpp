#include <NBodySim.hpp>

#include <cassert>
#include <iostream>

// Initial state
const int MOON_MASS     = 10;
const int EARTH_MASS    = 290000;
const int SUN_MASS      = 10000000;
const float GRAVITY_CONSTANT = 0.01f;
const float RATIO_DISTANCE = 100; // 1 => ratio*1 km

const int DISTANCE_EARTH_MOON = 7;
const int DISTANCE_EARTH_SUN = 80;

const glm::vec3 earth_position(0.f, DISTANCE_EARTH_SUN, -300.f);
const glm::vec3 moon_position(earth_position.x, earth_position.y+DISTANCE_EARTH_MOON, earth_position.z);


NBodySim::NBodySim():
    m_gravity(GRAVITY_CONSTANT, RATIO_DISTANCE),
    m_launched(false){

}

void NBodySim::init(const int WIDTH, const int HEIGHT){
    std::cout << "Initialize parameters" << std::endl;
    initWindow(WIDTH,HEIGHT);
    initPhysics();
    initGraphics();
}

void NBodySim::initWindow(const int WIDTH, const int HEIGHT){
    // Check if GLFW is ready
    assert(glfwInit());

    // Create the window and check it
    m_window = glfwCreateWindow(WIDTH, HEIGHT, "NBodySim", NULL, NULL);
    assert(m_window);

    // Make the OpenGL context current
    glfwMakeContextCurrent(m_window);

    // Init GLEW (warning ! must be initialised AFTER the openGL context)
    glewExperimental=true; // Needed in core profile

    GLenum error = glewInit();
    if (GLEW_OK != error)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
      glfwDestroyWindow(m_window);
      glfwTerminate();
    }
    glGetError();
}

void NBodySim::initPhysics(){
    // Initial velocities of the particles
    m_earthInitVelocity = glm::vec3(0.35f, 0, 0);
    m_moonInitVelocity = glm::vec3(0.55f, 0, 0);

    // Create three particles (Sun, Earth and Moon)
    m_particles.emplace_back(new FixedPoint(glm::vec3(0.f,0.f, -300), SUN_MASS));
    m_particles.emplace_back(new Particle(earth_position, EARTH_MASS));
    m_particles.emplace_back(new Particle(moon_position, MOON_MASS));
}

void NBodySim::initGraphics(){
    // Set the viewport
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    // Set the projection matrix
    m_stack.set(glm::perspective(45.0f, width / (float) height, 0.01f, 500.f));

    // Load shaders
    m_program.load("../../shaders/simple.vs.glsl","../../shaders/simple.fs.glsl");
    m_program.use();

    // Set the uniform location for the MVP matrix
    m_MVPLocation = m_program.getUniformLocation("MVP");

    // Load the Sphere.obj file
    m_sphere.load("../../assets/Sphere/sphere.obj");

    // Enable Depth test
    glEnable(GL_DEPTH_TEST);

    // Init GUI
    m_gui.init(m_window,"Parameters");
    m_gui.addParameter(m_fps, "fps","label='FPS' precision=1 help='Frame by seconds'");

    assert(m_particles.size() >= 3);
    m_gui.addParameter(m_particles[0]->getMass(), "sun_mass", "group='Sun' label='Mass' min=0 max=10000000 step=1000 help='Sun mass in kilograms'", true);

    m_gui.addParameter(m_particles[1]->getMass(), "earth_mass", "group='Earth' label='Mass' min=0 max=10000000 step=1000 help='Earth mass in kilograms'", true);
    m_gui.addParameter(m_earthInitVelocity.x, "earth_InitVelocity.x", "group='Earth' precision=3 label='Initial Velocity x' min=0 max=100 step=0.1 help='Earth initial velocity.x'", true);
    m_gui.addParameter(m_earthInitVelocity.y, "earth_InitVelocity.y", "group='Earth' precision=3 label='Initial Velocity y' min=0 max=100 step=0.1 help='Earth initial velocity.y'", true);
    m_gui.addParameter(m_particles[1]->getVelocity().x, "earth_velocity.x", "group='Earth' precision=3 label='Velocity x' help='Earth velocity.x'");
    m_gui.addParameter(m_particles[1]->getVelocity().y, "earth_velocity.y", "group='Earth' precision=3 label='Velocity y' help='Earth velocity.x'");

    m_gui.addParameter(m_particles[2]->getMass(), "moon_mass", "group='Moon' label='Mass' min=0 max=10000000 step=1000 help='Moon mass in kilograms'", true);
    m_gui.addParameter(m_moonInitVelocity.x, "moon_InitVelocity.x", "group='Moon' precision=3 label='Initial Velocity x' min=0 max=100 step=0.1 help='Moon initial velocity.x'", true);
    m_gui.addParameter(m_moonInitVelocity.y, "moon_InitVelocity.y", "group='Moon' precision=3 label='Initial Velocity y' min=0 max=100 step=0.1 help='Moon initial velocity.y'", true);
    m_gui.addParameter(m_particles[2]->getVelocity().x, "moon_velocity.x", "group='Moon' precision=3 label='Velocity x' help='Moon velocity.x'");
    m_gui.addParameter(m_particles[2]->getVelocity().y, "moon_velocity.y", "group='Moon' precision=3 label='Velocity y' help='Moon velocity.y'");

    m_gui.addParameter(m_gravity.getGravityForce(), "gravity_force", "group='Gravity' precision=3 label='Force' min=0 max=2 step=0.01 help='Gravity force in N/kg'", true);
    m_gui.addParameter(m_gravity.getDistRatio(), "dist_ratio", "group='Gravity' label='Distance Ratio' min=0 max=10000 step=10 help='Multiply every distances'", true);

    m_gui.addResetButton("Reset", this, "label='Reset' key=r");
    m_gui.addStartButton("Start", this, "label='Start' key=space");
}

void NBodySim::reset(){
    m_particles[1]->set(earth_position, glm::vec3(0));
    m_particles[2]->set(moon_position, glm::vec3(0));

    m_launched = false;
}

void NBodySim::start(){
    if (m_launched == false){
        m_launched = true;
        m_particles[1]->setVelocity(m_earthInitVelocity);
        m_particles[2]->setVelocity(m_moonInitVelocity);
    }
}

void NBodySim::run(){
    std::cout << "Simulation started" << std::endl;
    while (!glfwWindowShouldClose(m_window) && glfwGetKey(m_window, GLFW_KEY_ESCAPE)!=GLFW_PRESS)
    {
        double t = glfwGetTime();

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Launch simulation
        if (m_launched){
            // Calculate attraction force for all particles (O(n²) complexity)
            for (unsigned int i=0; i < m_particles.size(); ++i){
                for (unsigned int j=i+1; j < m_particles.size(); ++j){
                    m_gravity.setParticles(m_particles[i].get(), m_particles[j].get());
                    m_gravity.update();
                }
                m_particles[i]->update();
            }
        }


        // Render particles
        for (unsigned int i = 0 ; i < m_particles.size() ; ++i)
        {
            // Save current state of the stack
            m_stack.push();

            // Mult Projection Matrix by View Matrix
            m_stack.mult(m_camera.getViewMatrix());

            m_stack.translate(m_particles[i]->getPosition());

            if (i==0) m_stack.scale(glm::vec3(4));
            else if (i==1) m_stack.scale(glm::vec3(1.5));

            // Set uniform values
            glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, glm::value_ptr(m_stack.top()));

            // Render meshes
            m_sphere.render();

            // Reset stack
            m_stack.pop();
       }

        // Draw UI
        m_gui.draw();

        // Refresh window & events
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        double newTime = glfwGetTime();
        m_fps = 1.f/ (newTime - t);
    }

    terminate();

    std::cout << "Simulation ended" << std::endl;
}

void NBodySim::terminate(){
    // Delete particles
    m_particles.clear();

    // Terminate AntTweakBar
    m_gui.terminate();

    // Delete window and terminate GLFW
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
