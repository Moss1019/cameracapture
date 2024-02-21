#include "vertexbuffer.hpp"

#include "program.hpp"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

VertexBuffer::~VertexBuffer()
{
    if(m_handle != 0)
    {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }
}

GLuint VertexBuffer::handle() const
{
    return m_handle;
}

void VertexBuffer::fill(const std::vector<GLfloat> &data)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
}

void VertexBuffer::enableStreaming(const Program &p, const std::string &attribute, GLuint stride, GLuint offset)
{
    auto vertexPosition = glGetAttribLocation(p.handle(), attribute.c_str());
    glEnableVertexAttribArray(vertexPosition);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glVertexAttribPointer(vertexPosition, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(offset));
}