#include <iostream>
#include <thread>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "../image/image.hpp"
#include "../net/tcpsocket.hpp"
#include "../net/netaddress.hpp"
#include "../net/netmessage.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

using namespace cv;

int main(int argc, char **argv) 
{
    VideoCapture cap;
    cap.set(CAP_PROP_FRAME_WIDTH, 800);
    cap.set(CAP_PROP_FRAME_HEIGHT, 600);
    cap.open(0, CAP_ANY);
    MemoryOutputStream out;

    NetAddress remote("192.168.178.204", 8090);
    TcpSocket client(remote, true);
    client.connectToAddress();
    NetMessage msg;

    auto isRunning = true;

    while(isRunning)
    {
        NetMessage msg;
        client.receiveMessage(msg);
        if(client.inError())
        {
            std::cout << client.error() << "\n";
            isRunning = false;
            continue;
        }
        if(msg.type() == NetMessageType::InError)
        {
            isRunning = false;
            std::cout << "Client in error, exiting...\n";
        }
        else if(msg.type() == NetMessageType::ImageRequest)
        {
            Mat bgr;
            cap.read(bgr);
            Mat rgb;
            cvtColor(bgr, rgb, COLOR_BGR2RGB);

            auto size = rgb.total() * rgb.elemSize();
            Image img(size, rgb.type(), rgb.rows, rgb.cols, rgb.data);
            out.reset();
            img.writeTo(out);

            NetMessage imageResponse(NetMessageType::ImageResponse, out.data(), out.length());
            client.sendMessage(msg);
        }
        else
        {
            NetMessage res(NetMessageType::HeartBeat, nullptr, 0);
            client.sendMessage(msg);
        }
    }
    return 0;
}