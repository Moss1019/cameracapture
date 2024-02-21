#include "netmessage.hpp"

#include <cstring>
#include <iostream>

#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

NetMessage::NetMessage() 
{

}

NetMessage::NetMessage(NetMessageType type, const char *data, unsigned length)
    :m_type{type}, m_length{length}
{
    if(length > 0)
    {
        m_data = new char[length];
        std::memcpy(m_data, data, length);
    }
}

NetMessage::~NetMessage()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

NetMessageType NetMessage::type() const
{
    return m_type;
}

void NetMessage::setType(NetMessageType type)
{
    m_type = type;
}

void NetMessage::allocate(unsigned length)
{
    if(m_data != nullptr) 
    {
        delete[] m_data;
    }
    m_data = new char[length];
}

unsigned NetMessage::length() const
{
    return m_length;
}

const char *NetMessage::data() const
{
    return m_data;
}

char *NetMessage::buffer() const
{
    return m_data;
}

void NetMessage::writeTo(MemoryOutputStream &stream) const
{
    stream.write(static_cast<unsigned>(m_type));
    stream.write(m_length);
    if(m_length > 0)
    {
        stream.write(m_data, m_length);
    }
}   

void NetMessage::readFrom(MemoryInputStream &stream)
{
    unsigned underlaying = 0;
    stream.read(&underlaying);
    m_type = static_cast<NetMessageType>(underlaying);
    stream.read(&m_length);
    std::cout << "Length " << m_length << "\n";
    if(m_length > 0)
    {
        if(m_data != nullptr)
        {
            delete[] m_data;
        }
        m_data = new char[m_length];
        stream.read(m_data, m_length);
    }
}