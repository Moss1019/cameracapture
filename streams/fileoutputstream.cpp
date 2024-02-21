#include "fileoutputstream.hpp"

FileOutputStream::FileOutputStream(const std::string &filename)
{
    m_out.open(filename, std::ios_base::out | std::ios_base::binary);
}

FileOutputStream::~FileOutputStream()
{
    m_out.close();
}

void FileOutputStream::write(const char *data, unsigned length)
{
    m_out.write(data, length);
}
