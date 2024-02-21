#include "udpsocket.hpp"

#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include "netaddress.hpp"
#include "netmessage.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

UdpSocket::UdpSocket(NetAddress &local, bool binding)
{
    m_sock = socket(PF_INET, SOCK_DGRAM, 0);
    std::memcpy(&m_addr, local.addr(), local.addrLength());
    m_addrLength = local.addrLength();
    if(!binding)
    {
        fcntl(m_sock, F_SETFL, O_NONBLOCK);
    }
}

UdpSocket::~UdpSocket()
{
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
}

bool UdpSocket::inError() const
{
    return m_inError;
}

const std::string &UdpSocket::error() const
{
    return m_error;
}

void UdpSocket::bindToLocal()
{
    auto res = bind(m_sock, &m_addr, m_addrLength);
    if(res < 0)
    {
        m_inError = true;
        m_error = "Could not bind to local address";
    }
}

void UdpSocket::sendMessage(const NetMessage &message, NetAddress &remote)
{
    if(m_inError) 
    {
        return;
    }
    MemoryOutputStream out;
    message.writeTo(out);
    sendto(m_sock, out.data(), out.length(), 0, remote.addr(), remote.addrLength());
}

void UdpSocket::receiveMessage(NetMessage &message, NetAddress &remote)
{
    if(m_inError)
    {
        message.setType(NetMessageType::InError);
        return;
    }
    MemoryOutputStream out;
    const auto bufferSize = 1024 * 50;
    char buffer[bufferSize];
    auto isReading = true;
    while(isReading)
    {
        auto read = recvfrom(m_sock, buffer, bufferSize, 0, remote.addr(), remote.addrLengthPtr());
        if(read <= bufferSize)
        {
            isReading = false;
        }
        if(read == -1)
        {
            m_inError = true;
            m_error = "Remote closed connection";
            return;
        }
        out.write(buffer, read);
    }
    MemoryInputStream in(out.data(), out.length());
    message.readFrom(in);
}