cmake_minimum_required(VERSION 3.10)
project(NyanCatEvolution CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# OpenCV 3.x/4.x ships CMake config files (OpenCVConfig.cmake), so
# find_package works on every platform. On Windows, point CMake at the
# OpenCV build directory with -DOpenCV_DIR=<path> or the OPENCV_DIR
# environment variable if it is not found automatically.
find_package(OpenCV REQUIRED)

set(nyan_cat_SOURCE_FILES
    DnaBrush.cpp
    DnaCanvas.cpp
    DnaPoint.cpp
    DnaPolygon.cpp
    fitness.cpp
    main.cpp
    renderer.cpp
    tools.cpp
    )

add_executable(nyan_cat ${nyan_cat_SOURCE_FILES})
target_include_directories(nyan_cat PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(nyan_cat PRIVATE ${OpenCV_LIBS})
