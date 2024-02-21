#pragma once

#include <GL/glew.h>

class Shader;

class Program
{
private:
    GLuint m_handle = 0;

public:
    Program (const Program &other) = delete;

    Program operator=(const Program &other)  = delete;

    Program();

    ~Program();

    GLuint handle() const;

    void attachShader(const Shader &shader);

    void detachShader(const Shader &shader);

    void activate();
};
