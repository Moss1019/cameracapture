#pragma once

#include <string>
#include <fstream>

class FileOutputStream
{
private:
    std::ofstream m_out;

public:
    FileOutputStream(const FileOutputStream &other) = delete;

    FileOutputStream operator=(const FileOutputStream &other) = delete;

    FileOutputStream(const std::string &filename);

    ~FileOutputStream();

    void write(const char *data, unsigned length);
};
