#include "fileinputstream.hpp"

#include <iostream>

FileInputStream::FileInputStream(const std::string &filename)
{
    m_stream.open(filename, std::ios_base::in | std::ios_base::binary);
    m_stream.seekg(0, std::ios::end);
    m_length = m_stream.tellg();

    m_stream.seekg(0, std::ios::beg);
}

FileInputStream::~FileInputStream()
{
    m_stream.close();
}

unsigned FileInputStream::length() const
{
    return m_length;
}

unsigned FileInputStream::remaining() const
{
    return m_length - m_cursor;
}

void FileInputStream::read(char *data, unsigned length)
{
    m_stream.read(data, length);
    m_cursor += length;
}
