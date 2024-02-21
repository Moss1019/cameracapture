#include "shader.hpp"

#include "../streams/fileinputstream.hpp"

#include <iostream>

Shader::Shader(Type type, const std::string &filename)
{
    auto shaderType = GL_VERTEX_SHADER;
    switch(type) 
    {
        case Vertex:
            break;
        case Fragment:
            shaderType = GL_FRAGMENT_SHADER;
            break;
    }
    m_handle = glCreateShader(shaderType);
    FileInputStream fIn(filename);
    auto buffer = new char[fIn.length()];
    fIn.read(buffer, fIn.length());
    glShaderSource(m_handle, 1, &buffer, nullptr);
    glCompileShader(m_handle);
    auto status = GL_FALSE;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) 
    {
        const auto bufferSize = 1024;
        char buffer[bufferSize];
        auto length = 0;
        glGetShaderInfoLog(m_handle, bufferSize, &length, buffer);
        m_inError = true;
        m_error = std::string(buffer, length);
    }
    delete[] buffer;
}

Shader::~Shader()
{
    if(m_handle != 0)
    {
        glDeleteShader(m_handle);
        m_handle = 0;
    }
}

bool Shader::inError() const
{
    return m_inError;
}

const std::string &Shader::error() const
{
    return m_error;
}

GLuint Shader::handle() const
{
    return m_handle;
}