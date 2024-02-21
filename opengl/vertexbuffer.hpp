#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

class Program;

class VertexBuffer
{
private:
    GLuint m_handle = 0;
public:

    VertexBuffer();

    ~VertexBuffer();

    GLuint handle() const;

    void fill(const std::vector<GLfloat> &data);

    void enableStreaming(const Program &p, const std::string &attribute, GLuint stride, GLuint offset);
};

