#include <GUI.hpp>
#include <NBodySim.hpp>

void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods){TwEventMouseButtonGLFW(button, action);}
void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos){TwMouseMotion(int(xpos), int(ypos));}
void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset){TwEventMouseWheelGLFW(yoffset);}
void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods){TwEventKeyGLFW(key, action);}
void TwEventCharGLFW3(GLFWwindow* window, int codepoint){TwEventCharGLFW(codepoint, GLFW_PRESS);}

void GUI::init(GLFWwindow* window, const std::string& name){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(width, height);
    m_bar = TwNewBar(name.c_str());

    // directly redirect GLFW events to AntTweakBar
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
    glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
    glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
    glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);
}

void GUI::addParameter(float &parameter, const std::string &name, const std::string &definition, bool readandwrite){
    if (readandwrite)
        TwAddVarRW(m_bar, name.c_str(), TW_TYPE_FLOAT, &parameter,definition.c_str());
    else
        TwAddVarRO(m_bar, name.c_str(), TW_TYPE_FLOAT, &parameter,definition.c_str());
}

void TW_CALL ResetCallback(void *clientData)
{
    NBodySim* nbs = (NBodySim*)clientData;
    nbs->reset();
}

void TW_CALL StartCallback(void *clientData)
{
    NBodySim* nbs = (NBodySim*)clientData;
    nbs->start();
}

void GUI::addResetButton(const std::string& name, void* simulation, const std::string &definition){
    TwAddButton(m_bar,name.c_str(),ResetCallback, simulation, definition.c_str());
}

void GUI::addStartButton(const std::string& name, void* simulation, const std::string &definition){
    TwAddButton(m_bar,name.c_str(),StartCallback, simulation, definition.c_str());
}

void GUI::draw() const{
    TwDraw();
}

GUI::~GUI(){
    TwTerminate();
}
