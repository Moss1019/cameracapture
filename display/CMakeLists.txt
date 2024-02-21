cmake_minimum_required(VERSION 3.22)
project(cameradisplay)

set(CMAKE_CXX_STANDARD 17)

add_executable(cameradisplay
    main.cpp
    stb_image.cpp
    ../streams/memoryinputstream.cpp
    ../streams/memoryoutputstream.cpp
    ../streams/fileinputstream.cpp
    ../streams/fileoutputstream.cpp
    ../net/udpsocket.cpp
    ../net/tcpsocket.cpp
    ../net/netaddress.cpp
    ../net/netmessage.cpp
    ../opengl/shader.cpp
    ../opengl/program.cpp
    ../opengl/vertexbuffer.cpp
    ../opengl/indexbuffer.cpp
    ../image/image.cpp
)
target_link_libraries(cameradisplay PRIVATE GL GLEW glfw)
