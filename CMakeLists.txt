project(NyanCatEvolution)
cmake_minimum_required(VERSION 2.8.11.2)

if (MSVC)
    # Microsoft Visual Studio
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

    find_package(OpenCV REQUIRED)
    if (NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV not found")
    endif()

    # Sets include directories and additional dependencies for MSVC.
    # Renames OpenCV_{vars}to be compatible with non-MSVC env (OPENCV_{vars}).
    set(OPENCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
    set(OPENCV_LIBRARY_DIRS "${OpenCV_DIR}/x86/vc12/lib")
    set(OPENCV_LIBRARIES ${OpenCV_LIBRARIES})
else(MSVC)
    # non-MSVC environment.
    include(FindPkgConfig)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OPENCV REQUIRED opencv)
    if (NOT OPENCV_FOUND)
        message(FATAL_ERROR "OpenCV not found")
    endif()

    add_definitions(-std=c++11)
endif(MSVC)

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

link_directories(${OPENCV_LIBRARY_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})
add_executable(nyan_cat ${nyan_cat_SOURCE_FILES})
target_link_libraries(nyan_cat ${OpenCV_LIBRARIES})