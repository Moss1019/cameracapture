#include "netaddress.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <cstring>

NetAddress::NetAddress()
{
    
}

NetAddress::NetAddress (const NetAddress &other)
{
    m_addrLength = other.m_addrLength;
    std::memcpy(&m_addr, &other.m_addr, m_addrLength);
}

NetAddress::NetAddress(const std::string &ipAddress, short port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &addr.sin_addr);
    std::memcpy(&m_addr, &addr, sizeof(sockaddr));
    m_addrLength = sizeof(sockaddr);
}

sockaddr *NetAddress::addr()
{
    return &m_addr;
}

socklen_t NetAddress::addrLength()
{
    return m_addrLength;
}

socklen_t *NetAddress::addrLengthPtr()
{
    return &m_addrLength;
}
