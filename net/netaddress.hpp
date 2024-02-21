#pragma once

#include <sys/socket.h>

#include <string>

class NetAddress
{
private:
    sockaddr m_addr{};

    socklen_t m_addrLength = 0;
public:
    NetAddress operator=(const NetAddress &other) = delete;

    NetAddress();

    NetAddress (const NetAddress &other);

    NetAddress(const std::string &ipAddress, short port);

    sockaddr *addr();

    socklen_t addrLength();

    socklen_t *addrLengthPtr();
};
