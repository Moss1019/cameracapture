#pragma once

class MemoryInputStream;
class MemoryOutputStream;

enum NetMessageType: unsigned
{
    HeartBeat = 0,
    InError,
    ImageRequest,
    ImageResponse
};

class NetMessage
{
private:
    NetMessageType m_type;

    unsigned m_length;

    char *m_data = nullptr;
public:
    NetMessage(const NetMessage &other) = delete;

    NetMessage operator=(const NetMessage &other) = delete;

    NetMessage();

    NetMessage(NetMessageType type, const char *data, unsigned length);

    ~NetMessage();

    NetMessageType type() const;

    void allocate(unsigned length);

    void setType(NetMessageType type);

    unsigned length() const;

    const char *data() const;

    char *buffer() const;

    void interpret();

    void writeTo(MemoryOutputStream &stream) const;

    void readFrom(MemoryInputStream &stream);
};
