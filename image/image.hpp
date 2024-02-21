#pragma once

class MemoryInputStream;
class MemoryOutputStream;

class Image
{
private:
    unsigned long m_size = 0;

    int m_type = 0;

    int m_rows = 0;

    int m_cols = 0;

    char *m_data = nullptr;
public:
    Image(const Image &other) = delete;

    Image operator=(const Image &other) = delete;

    Image();

    Image(unsigned long size, int type, int rows, int cols, void *data);

    ~Image();

    unsigned long size() const;

    int type() const;

    int rows() const;

    int cols() const;

    const char *data() const;

    void writeTo(MemoryOutputStream &stream);

    void readFrom(MemoryInputStream &stream);
};
