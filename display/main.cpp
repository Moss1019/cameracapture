
#include <thread>
#include <cstring>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "../image/image.hpp"
#include "../opengl/shader.hpp"
#include "../opengl/program.hpp"
#include "../opengl/vertexbuffer.hpp"
#include "../opengl/indexbuffer.hpp"
#include "../net/tcpsocket.hpp"
#include "../net/netaddress.hpp"
#include "../net/netmessage.hpp"
#include "../streams/fileinputstream.hpp"
#include "../streams/fileoutputstream.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

auto width = 1024;
auto height = 1024;

void errorCallback(int error, const char *description)
{
    std::fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void sizeCallback(GLFWwindow *window, int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, newWidth, newHeight, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    auto res = glfwInit();
    if(res != 1)
    {
        std::cout << "failed to initialize glfw\n";
        return -1;
    }
        
    auto c = 3;
    FileInputStream fIn("test_img.bin");
    auto fileData = new char[fIn.length()];
    fIn.read(fileData, fIn.length());
    // auto fileData = stbi_load("image.png", &width, &height, &c, 0);

    unsigned long fileSize;
    MemoryInputStream mIn(fileData, fIn.length());
    mIn.read(&fileSize);
    mIn.read(&height);
    mIn.read(&width);
    int type = 0;
    mIn.read(&type);
    auto imageData = new char[fileSize];
    mIn.read(imageData, fileSize);

    auto window = glfwCreateWindow(width, height, "feed", nullptr, nullptr);
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, sizeCallback);
    glfwMakeContextCurrent(window);

    auto glewRes = glewInit();
    if(glewRes != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    Shader vertexS(Shader::Type::Vertex, "./vertex.glsl");
    if(vertexS.inError())
    {
        std::cout << "VS: " << vertexS.error() << "\n";
    }
    Shader fragmentS(Shader::Type::Fragment, "./fragment.glsl");
    if(fragmentS.inError())
    {
        std::cout << "FS: " << fragmentS.error() << "\n";
    }
    Program p;
    p.attachShader(vertexS);
    p.attachShader(fragmentS);

    // std::vector<GLfloat> vertices = {
    //     -1.0f, 1.0f, 
    //     -1.0f, -1.0f, 
    //     1.0f, -1.0f, 
    //     1.0f, 1.0f
    // };
    std::vector<GLfloat> vertices = {
        -1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };
    VertexBuffer vb;
    vb.fill(vertices);
    std::vector<GLuint> indices = {
        0,1,2,
        0,2,3
    };
    IndexBuffer ib;
    ib.fill(indices);

    p.activate();
    auto vpOffset = 0;
    vb.enableStreaming(p, "vertexPosition", 4 * sizeof(GLfloat), vpOffset);
    auto tcOffset = 2 * sizeof(GLfloat);
    vb.enableStreaming(p, "texCoord", 4 * sizeof(GLfloat), tcOffset);

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint pbo;
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, fileSize, 0, GL_STATIC_DRAW);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    GLuint smp = glGetAttribLocation(p.handle(), "smp");
    glUniform1i(smp, 0);

    NetAddress remote("192.168.178.204", 8080);
    TcpSocket socket(remote, true);
    socket.connectToAddress();

    const auto secondsPerFrame = 1000;
    
    while(!glfwWindowShouldClose(window))
    {
        NetMessage msg(NetMessageType::ImageRequest, nullptr, 0);
        socket.sendMessage(msg);
        NetMessage res;
        socket.receiveMessage(msg);
        if(socket.inError())
        {
            std::cout << socket.error() << "\n";
            glfwSetWindowShouldClose(window, GL_TRUE);
            continue;
        }
        MemoryInputStream mIn(msg.data(), msg.length());
        Image img;
        img.readFrom(mIn);
        if(img.rows() != height || img.cols() == width)
        {
            sizeCallback(window, img.cols(), img.rows());
        }
        GLubyte *ptr = (GLubyte *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        if(ptr) 
        {
            std::memcpy(ptr, img.data(), img.size());
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, ib.number(), GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();   
        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(secondsPerFrame));
    }

    glDeleteBuffers(1, &pbo);
    glDeleteTextures(1, &texture);
    // stbi_image_free(fileData);
    delete[] imageData;
    delete[] fileData;
    glfwTerminate();
    return 0;
}
