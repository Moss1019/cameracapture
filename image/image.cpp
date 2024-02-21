#include "image.hpp"

#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

#include <cstring>

Image::Image()
{

}

Image::Image(unsigned long size, int type, int rows, int cols, void *data)
    :m_size{size}, m_type{type}, m_rows{rows}, m_cols{cols}
{
    m_data = new char[size];
    std::memcpy(m_data, data, size);
}

Image::~Image()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

unsigned long Image::size() const
{
    return m_size;
}

int Image::type() const
{
    return m_type;
}

int Image::rows() const
{
    return m_rows;
}

int Image::cols() const
{
    return m_cols;
}

const char *Image::data() const
{
    return m_data;
}

void Image::writeTo(MemoryOutputStream &stream)
{
    stream.write(m_size);
    stream.write(m_type);
    stream.write(m_rows);
    stream.write(m_cols);
    stream.write(m_data, m_size);
}

void Image::readFrom(MemoryInputStream &stream)
{
    stream.read(&m_size);
    stream.read(&m_type);
    stream.read(&m_rows);
    stream.read(&m_cols);
    m_data = new char[m_size];
    stream.read(m_data, m_size);
}
