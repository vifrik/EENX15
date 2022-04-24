cmake_minimum_required(VERSION 3.18)
project(arucocpp)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_EXE_LINKER_FLAGS " -static-libstdc++") # for windows, for some reason Windows can't find std::vector

add_executable(arucocpp main.cpp cvwrapper.cpp Kalman.h Kalman.cpp Kalman.cpp Kalman.h)
target_link_libraries(arucocpp ${OpenCV_LIBS} Eigen3::Eigen)

add_executable(calibrate calibrate.cpp)
target_link_libraries(calibrate ${OpenCV_LIBS})

add_executable(writemarkers writemarkers.cpp)
target_link_libraries(writemarkers ${OpenCV_LIBS})

add_executable(photo photo.cpp)
target_link_libraries(photo ${OpenCV_LIBS})

add_executable(test test.cpp)
