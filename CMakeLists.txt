cmake_minimum_required(VERSION 3.1)
project(apriltag)

find_package(OpenCV QUIET)

include_directories(include)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_executable(apriltag_demo src/impl/apriltag_demo.c)
# target_link_libraries(apriltag_demo apriltag)

# opencv_demo
if(OpenCV_FOUND)
    add_executable(opencv_demo src/impl/opencv_demo.cc)
    target_link_libraries(opencv_demo apriltag ${OpenCV_LIBRARIES})
    set_target_properties(opencv_demo PROPERTIES CXX_STANDARD 11)
    install(TARGETS opencv_demo RUNTIME DESTINATION bin)
endif(OpenCV_FOUND)

