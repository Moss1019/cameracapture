#pragma once

#include <GL/glew.h>

#include <string>

class Shader
{
private:
    GLuint m_handle = 0;

    bool m_inError = false;

    std::string m_error;

public:
    enum Type
    {
        Vertex, 
        Fragment
    };

    Shader(const Shader &other) = delete;

    Shader operator=(const Shader &other) = delete;

    Shader(Type type, const std::string &filename);

    ~Shader();

    bool inError() const;

    const std::string &error() const;

    GLuint handle() const;
};
