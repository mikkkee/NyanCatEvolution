project(NyanCatEvolution)
cmake_minimum_required(VERSION 2.8.11.2)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv)

if (NOT OPENCV_FOUND)
  message(FATAL_ERROR "OpenCV not found")
endif()

include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})

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
add_definitions(-std=c++11)

add_executable(nyan_cat ${nyan_cat_SOURCE_FILES})

target_link_libraries(nyan_cat ${OPENCV_LIBRARIES})
