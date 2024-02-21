
#pragma once

class MemoryOutputStream
{
private:
    char *m_data = nullptr;

    unsigned m_length = 0;

    unsigned m_capacity = 0;

    void reallocate(unsigned newCapacity);
public:
    MemoryOutputStream(const MemoryOutputStream &other) = delete;

    MemoryOutputStream operator=(const MemoryOutputStream &other) = delete;

    MemoryOutputStream();

    ~MemoryOutputStream();

    const char *data() const;

    unsigned length() const;

    void reset();

    void write(int data);

    void write(unsigned data);

    void write(double data);

    void write(unsigned long);

    void write(const void *data, unsigned length);

    void write(void *data, unsigned length);
};
