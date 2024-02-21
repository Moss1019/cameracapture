#pragma once

#include <GL/glew.h>

class Texture
{
private:
    GLuint m_handle = 0;

    GLuint m_slot = 0;
public:
    Texture(unsigned char *data, GLuint slot);

    ~Texture();

    GLuint handle() const;

    void bind();
};