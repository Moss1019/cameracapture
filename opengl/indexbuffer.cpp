#include "indexbuffer.hpp"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

IndexBuffer::~IndexBuffer()
{
    if(m_handle != 0)
    {
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }
}

GLuint IndexBuffer::handle() const
{
    return m_handle;
}

unsigned IndexBuffer::number() const
{
    return m_number;
}

void IndexBuffer::fill(const std::vector<GLuint> &data)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
    m_number = data.size();
}