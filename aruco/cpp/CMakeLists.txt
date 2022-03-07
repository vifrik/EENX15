cmake_minimum_required(VERSION 3.21)
project(arucocpp)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# when compiling for release, use add_executable(arucocpp main.cpp) ??
add_executable(arucocpp main.cpp cvwrapper.cpp cvwrapper.h tcpclient.cpp tcpclient.h math.h writevec.h defines.h)
target_link_libraries(arucocpp ${OpenCV_LIBS})

add_executable(calibrate calibrate.cpp)
target_link_libraries(calibrate ${OpenCV_LIBS})

add_executable(writemarkers writemarkers.cpp)
target_link_libraries(writemarkers ${OpenCV_LIBS})

add_executable(test test.cpp)
