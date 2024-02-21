#pragma once

#include <sys/socket.h>

#include <string>

class NetAddress;
class NetMessage;

class TcpSocket
{
private:
    int m_sock = -1;

    sockaddr m_addr{};

    socklen_t m_addrLength;

    bool m_inError = false;

    std::string m_error;
public:
    TcpSocket(const TcpSocket &other) = delete;

    TcpSocket operator=(const TcpSocket &other) = delete;

    TcpSocket(int sock, bool binding);

    TcpSocket(NetAddress &local, bool binding);

    ~TcpSocket();

    bool inError() const;

    const std::string &error() const;

    socklen_t addrLength() const;

    void bindToAddress();

    int acceptConnection(NetAddress &remote);

    void connectToAddress();

    void sendMessage(const NetMessage &msg);

    int receiveMessage(NetMessage &msg);
};
