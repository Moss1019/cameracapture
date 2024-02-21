cmake_minimum_required(VERSION 3.25)
project(cameracapture)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(/usr/local/include/opencv4)

add_executable(cameracapture
    main.cpp
    ../streams/memoryinputstream.cpp
    ../streams/memoryoutputstream.cpp
    ../net/udpsocket.cpp
    ../net/tcpsocket.cpp
    ../net/netaddress.cpp
    ../net/netmessage.cpp
    ../image/image.cpp
)
target_link_libraries(cameracapture PRIVATE /usr/local/lib/libopencv_core.4.9.0.dylib)
target_link_libraries(cameracapture PRIVATE /usr/local/lib/libopencv_highgui.4.9.0.dylib)
target_link_libraries(cameracapture PRIVATE /usr/local/lib/libopencv_videoio.4.9.0.dylib)
