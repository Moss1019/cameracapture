#pragma once

#include <sys/socket.h>

#include <string>

class NetAddress;
class NetMessage;

class UdpSocket
{
private:
    int m_sock;

    sockaddr m_addr{};

    socklen_t m_addrLength = 0;

    bool m_inError = false;

    std::string m_error;
public:
    UdpSocket(const UdpSocket &other) = delete;

    UdpSocket operator=(const UdpSocket &other) = delete;

    UdpSocket(NetAddress &local, bool binding);

    ~UdpSocket();

    bool inError() const;

    const std::string &error() const;

    void bindToLocal();

    void sendMessage(const NetMessage &message, NetAddress &remote);

    void receiveMessage(NetMessage &message, NetAddress &remote);
};