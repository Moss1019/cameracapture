#include "texture.hpp"

Texture::Texture(unsigned char *data, GLuint slot)
    :m_slot{slot}
{
    
}

Texture::~Texture()
{
    if(m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
    }
}

GLuint Texture::handle() const
{
    return m_handle;
}

void Texture::bind()
{

}
