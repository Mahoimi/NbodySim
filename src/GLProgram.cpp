#include <GLProgram.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

GLProgram::GLProgram():m_programID(0){

}

static std::string readFile(const char* filePath) {
    std::ifstream input(filePath);
    assert(input);

    std::stringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

GLuint GLProgram::load(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile){
        assert(vertexShaderFile != nullptr);
        assert(fragmentShaderFile != nullptr);

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        assert(glIsShader(VertexShaderID) == GL_TRUE);
        assert(glIsShader(FragmentShaderID) == GL_TRUE);

    //Read the shaders files
    std::string vscode = readFile(vertexShaderFile);
    const char* VertexShaderCode = vscode.c_str();
    std::string fscode = readFile(fragmentShaderFile);
    const char* FragmentShaderCode = fscode.c_str();

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    std::cout << "Compiling shader : " << vertexShaderFile << std::endl;
    glShaderSource(VertexShaderID, 1, &VertexShaderCode , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    if(Result == GL_FALSE) {
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        char* log = new char[InfoLogLength];
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,log);
        std::cerr << "Vexter Shader error:" << log << std::endl;
        std::cerr << VertexShaderCode << std::endl;

        delete [] log;
        return 0;
    }

    // Compile Fragment Shader
    std::cout << "Compiling shader : " <<  fragmentShaderFile << std::endl;
    glShaderSource(FragmentShaderID, 1, &FragmentShaderCode , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    if(Result == GL_FALSE) {
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        char* log = new char[InfoLogLength];
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, log);
        std::cerr << "Fragment Shader error:" << log << std::endl;
        std::cerr << FragmentShaderCode << std::endl;

        delete [] log;
        return 0;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, VertexShaderID);
    glAttachShader(m_programID, FragmentShaderID);

    GLuint GeometryShaderID;
    if(geometryShaderFile) {
        GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

                assert(glIsShader(GeometryShaderID) == GL_TRUE);

        std::string gscode = readFile(geometryShaderFile);
        const char* GeometryShaderCode = gscode.c_str();

        // Compile Geometry Shader
        std::cout << "Compiling shader :" <<  geometryShaderFile << std::endl;
        glShaderSource(GeometryShaderID, 1, &GeometryShaderCode , NULL);
        glCompileShader(GeometryShaderID);

        // Check Geometry Shader
        glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
        if(Result == GL_FALSE) {
            glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            char* log = new char[InfoLogLength];
            glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL,log);
            std::cerr << "Geometry Shader error:" << log << std::endl;
            std::cerr << GeometryShaderCode << std::endl;

            delete [] log;
            return 0;
        }

        glAttachShader(m_programID, GeometryShaderID);
    }

    // Link the program
    fprintf(stdout, "Linking program\n");
    glLinkProgram(m_programID);

    // Check the program
    glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
    if(Result == GL_FALSE) {
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
        glGetProgramInfoLog(m_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
        return 0;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    if(geometryShaderFile) {
        glDeleteShader(GeometryShaderID);
    }

    return m_programID;
}

GLProgram::~GLProgram(){
    if (m_programID) glDeleteProgram(m_programID);
}
