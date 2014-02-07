#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>
#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

class GUI {
private:
    TwBar* m_bar;

public:
    GUI():m_bar(nullptr){}
    void init(GLFWwindow *window, const std::string& name);
    void addParameter(float& parameter, const std::string& name, const std::string& definition, bool readandwrite = false);
    void addResetButton(const std::string& name, void *data, const std::string &definition);
    void addStartButton(const std::string& name, void *data, const std::string &definition);
    void draw() const;
    ~GUI();
};


#endif // GUI_HPP
