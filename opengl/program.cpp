#include "program.hpp"

#include "shader.hpp"

Program::Program()
{
    m_handle = glCreateProgram();

}

Program::~Program()
{
    if(m_handle != 0)
    {
        glDeleteProgram(m_handle);
        m_handle = 0;
    }
}

GLuint Program::handle() const
{
    return m_handle;
}

void Program::attachShader(const Shader &shader)
{
    glAttachShader(m_handle, shader.handle());
}

void Program::detachShader(const Shader &shader)
{
    glDetachShader(m_handle, shader.handle());
}

void Program::activate()
{
    glLinkProgram(m_handle);
    glUseProgram(m_handle);
}