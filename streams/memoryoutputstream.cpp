#include "memoryoutputstream.hpp"

#include <cstring>
#include <algorithm>

MemoryOutputStream::MemoryOutputStream()
{
    reallocate(1024 * 1024);
}

MemoryOutputStream::~MemoryOutputStream()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
    }
}

const char *MemoryOutputStream::data() const
{
    return m_data;
}

unsigned MemoryOutputStream::length() const
{
    return m_length;
}

void MemoryOutputStream::reset()
{
    m_length = 0;
}

void MemoryOutputStream::write(int data)
{
    write(&data, sizeof(int));
}

void MemoryOutputStream::write(unsigned data)
{
    write(&data, sizeof(unsigned));
}

void MemoryOutputStream::write(double data)
{
    write(&data, sizeof(double));
}

void MemoryOutputStream::write(unsigned long data)
{
    write(&data, sizeof(unsigned long));
}

void MemoryOutputStream::write(void *data, unsigned length)
{
    auto newLength = m_length + length;
    if(m_capacity < newLength)
    {
        reallocate(std::max(newLength, m_capacity * 2));
    }
    std::memcpy(m_data + m_length, data, length);
    m_length = newLength;
}

void MemoryOutputStream::write(const void *data, unsigned length)
{
    auto newLength = m_length + length;
    if(m_capacity < newLength)
    {
        reallocate(std::max(newLength, m_capacity * 2));
    }
    std::memcpy(m_data + m_length, data, length);
    m_length = newLength;
}

void MemoryOutputStream::reallocate(unsigned newCapacity)
{
    m_capacity = newCapacity;
    auto data = new char[m_capacity];
    if(m_data != nullptr)
    {
        std::memcpy(data, m_data, m_length);
        delete[] m_data;
    }
    m_data = data;
}