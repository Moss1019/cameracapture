#pragma once

#include <vector>

#include <GL/glew.h>

class IndexBuffer
{
private:
    GLuint m_handle = 0;

    unsigned m_number = 0;
public:
    IndexBuffer();

    ~IndexBuffer();

    GLuint handle() const;

    unsigned number() const;

    void fill(const std::vector<GLuint> &data);
};