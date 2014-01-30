#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <tr1/memory>

#include <FreeFlyCamera.hpp>
#include <MatrixStack.hpp>
#include <MeshRenderer.hpp>
#include <GLProgram.hpp>
#include <Particle.hpp>
#include <FixedPoint.hpp>
#include <GravityLink.hpp>

int main(){

    // Check if GLFW is ready
    if(!glfwInit())
    {
        return EXIT_FAILURE;
    }

    // Create the window and check it
    GLFWwindow* window = glfwCreateWindow(1280, 720, "NBodySim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Init GLEW (warning ! must be initialised AFTER the openGL context)
    GLenum error = glewInit();
    if (GLEW_OK != error)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
      glfwDestroyWindow(window);
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    // Physics objects
    srand(1);

    std::vector<std::tr1::shared_ptr<PMat>> particles;
    particles.emplace_back(new FixedPoint(glm::vec3(0.f,0.f, -300), 100000));

    for (unsigned int i = 0; i < 500; ++i){
        glm::vec3 posrand(glm::vec3(rand()%200-100, rand()%200-100, -300));
        float xrand = rand()%10+1;
        glm::vec3 velocity(xrand, -posrand.x * xrand / posrand.y, 0);


        if (posrand.y >= 0) {
            velocity.x = abs(velocity.x);
        }
        else{
            velocity.x = - abs(velocity.x);
        }

        if (posrand.x >= 0) {
            velocity.y = -abs(velocity.y);
        }
        else {
            velocity.y = abs(velocity.y);
        }

        velocity = glm::normalize(velocity);

        particles.emplace_back(new Particle(posrand,velocity,1));
    }
    GravityLink gravity;

    // Graphics objects & parameters

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    FreeFlyCamera camera;
    MatrixStack stack;
    stack.set(glm::perspective(45.0f, width / (float) height, 0.01f, 1000.f));

    GLProgram program;
    program.load("../../shaders/simple.vs.glsl","../../shaders/simple.fs.glsl");
    program.use();

    GLuint MVPLocation = program.getUniformLocation("MVP");

    MeshRenderer sphere;
    sphere.load("../../assets/Sphere/sphere.obj");

    double xpos, ypos, prev_xpos, prev_ypos;
    glfwGetCursorPos(window, &prev_xpos, &prev_ypos);

    bool space = false;

    // Main loop
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS)
    {
        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Keyboard input
        if (glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS) {
            camera.moveFront(1.f);
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN)==GLFW_PRESS){
            camera.moveFront(-1.f);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT)==GLFW_PRESS) {
            camera.moveLeft(1.f);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT)==GLFW_PRESS){
            camera.moveLeft(-1.f);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE)==GLFW_PRESS){
            space=true;
        }

        // Mouse input
        glfwGetCursorPos(window, &xpos, &ypos);

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
           //Get mouse positions
           camera.rotateLeft(-0.4*(xpos-prev_xpos));
           camera.rotateUp(-0.4*(ypos-prev_ypos));
        }

        prev_xpos=xpos;
        prev_ypos=ypos;

        if (space){
            for (unsigned int i=0; i < particles.size(); ++i){
                for (unsigned int j=i+1; j < particles.size(); ++j){
                    gravity.setParticles(particles[i].get(), particles[j].get());
                    gravity.update();
                }
            }
            for (unsigned int i=0; i < particles.size(); ++i){
                particles[i]->update();
            }
        }

        for (unsigned int i = 0 ; i < particles.size() ; ++i)
        {

            // Save current state of the stack
            stack.push();

            // Mult Projection Matrix by View Matrix
            stack.mult(camera.getViewMatrix());

            stack.translate(particles[i]->getPosition());

            // Set uniform values
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(stack.top()));

            // Render meshes
            sphere.render();

            // Reset stack
            stack.pop();
        }

        // Refresh window & events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free memory
    particles.clear();

    glfwDestroyWindow(window);
    glfwTerminate();
	return EXIT_SUCCESS;
}
