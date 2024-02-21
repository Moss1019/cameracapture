#include "memoryinputstream.hpp"

#include <cstring>

#include <iostream>

MemoryInputStream::MemoryInputStream(const char *data, unsigned length)
    :m_length{length}
{
    if(length > 0) 
    {
        std::cout << "stream Length " << length << "\n";
        m_data = new char[length];
        std::memcpy(m_data, data, length);
    }
}

MemoryInputStream::~MemoryInputStream()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

unsigned MemoryInputStream::remaining() const
{
    return m_length - m_cursor;
}

void MemoryInputStream::setData(const char *data, unsigned length)
{
    if(m_data != nullptr)
    {
        delete[] m_data;
    }
    m_data = new char[length];
    std::memcpy(m_data, data, length);
    m_length = length;
    m_cursor = 0;
}

void MemoryInputStream::read(int *buffer)
{
    read(buffer, sizeof(int));
}

void MemoryInputStream::read(unsigned *buffer)
{
    read(buffer, sizeof(unsigned));
}

void MemoryInputStream::read(double *buffer)
{
    read(buffer, sizeof(double));
}

void MemoryInputStream::read(unsigned long *buffer)
{
    read(buffer, sizeof(unsigned long));
}

void MemoryInputStream::read(void *buffer, unsigned length)
{
    std::memcpy(buffer, m_data + m_cursor, length);
    m_cursor += length;
}