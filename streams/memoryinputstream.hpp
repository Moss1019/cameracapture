#pragma once

class MemoryInputStream
{
private:
    char *m_data = nullptr;

    unsigned m_length = 0;

    unsigned m_cursor = 0;
public:
    MemoryInputStream(const MemoryInputStream &other) = delete;

    MemoryInputStream operator=(const MemoryInputStream &other) = delete;

    MemoryInputStream(const char *data, unsigned length);

    ~MemoryInputStream();

    unsigned remaining() const;

    void setData(const char *data, unsigned length);

    void read(int *buffer);

    void read(unsigned *buffer);

    void read(double *buffer);

    void read(unsigned long *buffer);

    void read(void *buffer, unsigned length);
};
