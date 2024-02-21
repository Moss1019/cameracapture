#pragma once

#include <string>
#include <fstream>

class FileInputStream
{
private:
    std::ifstream m_stream;

    unsigned m_length = 0;

    unsigned m_cursor = 0;

public:
    FileInputStream(const FileInputStream &other) = delete;

    FileInputStream operator=(const FileInputStream &other) = delete;

    FileInputStream(const std::string &filename);

    ~FileInputStream();

    unsigned length() const;

    unsigned remaining() const;

    void read(char *data, unsigned length);
};
