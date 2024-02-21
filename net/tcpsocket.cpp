#include "tcpsocket.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "netaddress.hpp"
#include "netmessage.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

TcpSocket::TcpSocket(int sock, bool binding)
    :m_sock{sock}
{
    if(!binding)
    {
        fcntl(m_sock, F_SETFL, O_NONBLOCK);
    }
}

TcpSocket::TcpSocket(NetAddress &local, bool binding)
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    m_addrLength = local.addrLength();
    std::memcpy(&m_addr, local.addr(), local.addrLength());
    if(!binding)
    {
        fcntl(m_sock, F_SETFL, O_NONBLOCK);
    }
}

TcpSocket::~TcpSocket()
{
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::error() const
{
    return m_error;
}

socklen_t TcpSocket::addrLength() const
{
    return m_addrLength;
}

void TcpSocket::bindToAddress()
{
    auto res = bind(m_sock, &m_addr, m_addrLength);
    if(res < 0)
    {
        m_inError = true;
        m_error = "Could not bind to local address";
        return;
    }
    listen(m_sock, 2);
}

int TcpSocket::acceptConnection(NetAddress &remote)
{
    return accept(m_sock, remote.addr(), remote.addrLengthPtr());
}

void TcpSocket::connectToAddress()
{
    auto res = connect(m_sock, &m_addr, m_addrLength);
    if(res < 0)
    {
        m_inError = true;
        m_error = "Could not connect to remote host";
    }
}

void TcpSocket::sendMessage(const NetMessage &msg)
{
    MemoryOutputStream out;
    msg.writeTo(out);
    const unsigned maxBufferSize = 65535;
    auto length = out.length();
    auto chunks = static_cast<unsigned>(std::ceil(1.0 * length / maxBufferSize));
    auto totalSent = 0;
    send(m_sock, &length, sizeof(unsigned), 0);
    while(totalSent < length)
    {
        auto sent = send(m_sock, out.data() + totalSent, std::min(maxBufferSize, length - totalSent), 0);
        totalSent += sent;
    }
}

int TcpSocket::receiveMessage(NetMessage &msg)
{
    const auto bufferSize = 65535;
    char buffer[bufferSize];
    auto isReading = true;
    auto totalRead = 0;
    MemoryOutputStream out;
    unsigned length = 0;
    recv(m_sock, &length, sizeof(unsigned), 0);
    while(isReading)
    {
        while(totalRead < length)
        {
            auto read = recv(m_sock, buffer, bufferSize, 0);
            totalRead += read;
            if(read <= bufferSize)
            {
                isReading = false;
            }
            if(read <= 0)
            {
                m_inError = true;
                m_error = "Remote connection closed";
                return -1;
            }
            out.write(buffer, read);
            // writer.write(buffer, read);
        }
        if(totalRead < length)
        {
            std::cout << "invalid length\n";
        }
        isReading = false;
    }
    MemoryInputStream in(out.data(), out.length());
    msg.readFrom(in);
    return totalRead;
}